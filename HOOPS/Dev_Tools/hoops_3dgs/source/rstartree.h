#ifndef RTREE_H
#define RTREE_H

// (Trimmed) original header
// \class RTree
// Implementation of R*Tree, a multidimensional bounding rectangle tree.
// Example usage: For a 3-dimensional tree use RTree<Object*, float, 3> myTree;
//
// This modified, templated C++ version by Greg Douglas at Auran (http://www.auran.com)
//
// DATATYPE Referenced data, should be int, void *, obj* etc. no larger than sizeof<void *> and simple type
//
// NOTES: Inserting and removing data requires the knowledge of its constant Minimal Bounding Rectangle.
//

// New:
// Class RStarTree
// Heavily modified to assume Hoops components, lots of cruft discarded
// basic usage: RStarTree<Item *, Cuboid_3D, float> rtree_of_items;
template <typename DATATYPE,
		  template <typename XF> class BOUNDING_CLASS,
		  typename F,
		  int MAXNODES = 8, int MINNODES = MAXNODES / 2>
class RStarTree {
	protected:
		typedef BOUNDING_CLASS<F> BOUNDING;
		struct Node;
		// data if parent is level 0, else pointer to a child
		struct Branch {
			BOUNDING	m_bounds;
			Node *		m_child;
			DATATYPE	m_data;
		};

		struct Node {
			int			m_count;
			int			m_level;
			Branch		m_branch[MAXNODES];

			Node() : m_count (0), m_level (-1) {}
			bool	IsLeaf()						{ return (m_level == 0); }
			BOUNDING Volume() {
					BOUNDING	bounds = m_branch[0].m_bounds;
					for (int i = 1; i < m_count; ++i)
						 bounds.Merge (m_branch[i].m_bounds);
					return bounds;
				}
		};
	
		// A linked list of nodes for reinsertion after a delete operation
		struct Node_List {
			Node_List *	m_next;
			Node *		m_node;
		};

		// Variables for finding a split partition
		struct Patitioning {
			enum { UNASSIGNED = -1 };

			Branch			m_branchBuf[MAXNODES+1];	// branches from one full node plus the new one
			int				m_partition[MAXNODES+1];	// choice of how they are distributed
			int				m_count[2];
			BOUNDING		m_cover[2];
			F				m_volume[2];

			BOUNDING		m_total_bounds;
			F				m_total_volume;

			Partitioning (Node * node, Branch const * branch) {
				for (int i = 0; i < MAXNODES; ++i) {
					m_branchBuf[i] = node->m_branch[i];
					m_partition[i] = UNASSIGNED;
				}
				m_branchBuf[MAXNODES] = branch;
				m_partition[MAXNODES] = UNASSIGNED;

				m_count[0] = m_count[1] = 0;
				m_volume[0] = m_volume[1] = 0;

				m_total_bounds = m_branchBuf[0].m_bounds;
				for (int i = 1; i < MAXNODES+1; ++i)
					m_total_bounds.Merge(m_branchBuf[i].m_bounds);
				m_total_volume = m_total_bounds.Volume();
			}
		};

		Node *			m_root;                                    // Root of tree
		Memory_Pool *	m_pool;
 
	public:
		typedef bool (*Callback_Function)(DATATYPE, void *);

		// Iterator is not remove safe.
		class Iterator {
			private:
				enum { MAX_STACK = 32 };	//  Max stack size. Allows almost n^32 where n is number of branches in node
		
				struct StackElement {
					Node *	m_node;
					int		m_branchIndex;
				};

				StackElement	m_stack[MAX_STACK];			// Stack as we are doing iteration instead of recursion
				int				m_tos;						// Top Of Stack index
	
				friend class RStarTree; // Allow hiding of non-public functions while allowing manipulation by logical owner
		
			public:
				Iterator()									{ Init(); }
				~Iterator()									{ }
		
				// Is iterator invalid
				bool IsNull()								{ return (m_tos <= 0); }

				// Is iterator pointing to valid data
				bool IsNotNull()							{ return (m_tos > 0); }

				// Access the current data element. Caller must be sure iterator is not null first.
				DATATYPE & operator*() {
					ASSERT(IsNotNull());
					StackElement &	curTos = m_stack[m_tos - 1];
					return curTos.m_node->m_branch[curTos.m_branchIndex].m_data;
				} 

				// Access the current data element. Caller must be sure iterator is not null first.
				DATATYPE const & operator*() const {
					ASSERT(IsNotNull());
					StackElement &	curTos = m_stack[m_tos - 1];
					return curTos.m_node->m_branch[curTos.m_branchIndex].m_data;
				} 

				// Find the next data element
				bool operator++()							{ return FindNextData(); }

				// Get the bounds for this node
				void GetBounds(BOUNDING & bounds) {
					ASSERT(IsNotNull());
					StackElement &	curTos = m_stack[m_tos - 1];
					Branch &		curBranch = curTos.m_node->m_branch[curTos.m_branchIndex];
					bounds = curBranch.m_bounds;
				}

			private:
				// Reset iterator
				void Init()									{ m_tos = 0; }

				// Find the next data element in the tree (For internal use only)
				bool FindNextData() {
					for (;;) {
						if (m_tos <= 0)
							return false;

						StackElement curTos = Pop();	// Copy stack top cause it may change as we use it

						if (curTos.m_node->IsLeaf()) {
							// Keep walking through data while we can
							if (curTos.m_branchIndex+1 < curTos.m_node->m_count) {
								// There is more data, just point to the next one
								Push(curTos.m_node, curTos.m_branchIndex + 1);
								return true;
							}
							// No more data, so it will fall back to previous level
						}
						else {
							if (curTos.m_branchIndex+1 < curTos.m_node->m_count) {
								// Push sibling on for future tree walk
								// This is the 'fall back' node when we finish with the current level
								Push(curTos.m_node, curTos.m_branchIndex + 1);
							}
							// Since cur node is not a leaf, push first of next level to get deeper into the tree
							Node * nextLevelnode = curTos.m_node->m_branch[curTos.m_branchIndex].m_child;
							Push(nextLevelnode, 0);
					
							// If we pushed on a new leaf, exit as the data is ready at TOS
							if (nextLevelnode->IsLeaf())
								return true;
						}
					}
				}

				// Push node and branch onto iteration stack (For internal use only)
				void Push(Node * node, int a_branchIndex) {
					m_stack[m_tos].m_node = node;
					m_stack[m_tos].m_branchIndex = a_branchIndex;
					++m_tos;
					ASSERT(m_tos <= MAX_STACK);
				}
		
				// Pop element off iteration stack (For internal use only)
				StackElement & Pop() {
					ASSERT(m_tos > 0);
					--m_tos;
					return m_stack[m_tos];
				}
		};

		// Get 'first' for iteration
		void GetFirst(Iterator & a_it) {
			a_it.Init();
			Node * first = m_root;
			while (first) {
				if (!first->IsLeaf() && first->m_count > 1)
					a_it.Push(first, 1); // Descend sibling branch later
				else if (first->IsLeaf()) {
					if (first->m_count)
						a_it.Push(first, 0);
					break;
				}
				first = first->m_branch[0].m_child;
			}
		}  

		// Get Next for iteration
		void GetNext(Iterator & a_it)				{ ++a_it; }

		// Is iterator null, or at end?
		bool IsNull(Iterator & a_it)				{ return a_it.IsNull(); }

		// Get object at iterator position
		DATATYPE & GetAt(Iterator& a_it)			{ return *a_it; }

	protected:
		Node * AllocNode()							{ return new Node; }
		void FreeNode(Node * node)					{ delete node; }

		Node_List * AllocListNode()					{ return new Node_List; }
		void FreeListNode(Node_List * listNode)		{ delete listNode; }



		bool insert_recursive(Branch const & branch, Node * node, Node ** new_node, int level) {
				ASSERT(level >= 0 && level <= node->m_level);

				if (node->m_level == level)
					return AddBranch(branch, node, new_node);

				ASSERT (node->m_level > level);
				int		index = PickBranch(branch.m_bounds, node);
				// recursively insert this record into the picked branch
				Node *	otherNode = null;
				if (!insert_recursive(branch, node->m_branch[index].m_child, &otherNode, level)) {
					// not split, just fix bounds
					node->m_branch[index].m_bounds.Merge(branch.m_bounds);
					return false;
				}
				// repair this one
				node->m_branch[index].m_bounds = node->m_branch[index].m_child->Volume();
				// and insert the new one
				Branch new_branch;
				new_branch.m_child = otherNode;
				new_branch.m_bounds = NotherNode->Volume();
				return AddBranch(new_branch, node, new_node);
			}
		bool insert_item(Branch const & branch, Node ** root, int level) {
				ASSERT(level >= 0 && level <= (*root)->m_level);
				Node *	newNode = null;
				if (!insert_recursive(branch, *root, &newNode, level))
					return false;

				// split, put pieces under a new root
				Node *	newRoot = AllocNode();
				newRoot->m_level = (*root)->m_level + 1;

				Branch	new_branch;
				new_branch.m_bounds = (*root)->Volume();
				new_branch.m_child = *root;
				AddBranch(new_branch, newRoot);

				new_branch.m_bounds = newNode->Volume();
				new_branch.m_child = newNode;
				AddBranch(new_branch, newRoot);

				*root = newRoot;
				return true;
			}


		bool AddBranch(Branch const & branch, Node * node, Node ** new_node = 0) {
				if (node->m_count < MAXNODES) {	// add
					node->m_branch[node->m_count] = branch;
					++node->m_count;
					return false;
				}
				else {								// split
					ASSERT(new_node);
					SplitNode(node, branch, new_node);
					return true;
				}
			}
		void SplitNode(Node * node, Branch const & branch, Node ** new_node) {
				Patitioning		parVars (node, branch);
				ChoosePartition(parVars);

				*new_node = AllocNode();
				(*new_node)->m_level = node->m_level;

				node->m_count = 0;
				LoadNodes(node, *new_node, parVars);
	
				ASSERT((node->m_count + (*new_node)->m_count) == parVars->m_total);
			}
		void DisconnectBranch(Node * node, int index){
				ASSERT(node && index >= 0 && index < MAXNODES);
				ASSERT(node->m_count > 0);
				// not sorted, so bring one down to fill
				node->m_branch[index] = node->m_branch[node->m_count--];
			}
		int PickBranch(BOUNDING const & bounds, Node * node) {
				F		smallest_increase = std::numeric_limits<F>::max();
				F		smallest_volume = std::numeric_limits<F>::max();
				int		best = -1;
				for (int i=0; i < node->m_count; ++i) {
					BOUNDING const &	curRect = node->m_branch[i].m_bounds;
					BOUNDING			tempRect = Union (bounds, curRect);
					F					volume = curRect.Volume();
					F					increase = tempRect.Volume() - volume;
					if (increase < smallest_increase || increase == smallest_increase && volume < smallest_volume) {
						best = i;
						smallest_volume = volume;
						smallest_increase = increase;
					}
				}
				return best;
			}
		void ChoosePartition(Patitioning & partition) {
				F		biggestDiff;
				int		group, chosen, betterGroup;
	
				PickSeeds(partition);

				while (partition.m_count[0] + partition.m_count[1] < MAXNODES + 1 &&
					   partition.m_count[0] < MAXNODES + 1 - MINNODES &&
					   partition.m_count[1] < MAXNODES + 1 - MINNODES) {
					biggestDiff = -1;
					for (int i=0; i < MAXNODES + 1; ++i) {
						if (Patitioning::UNASSIGNED == partition.m_partition[i]) {
							BOUNDING &		curRect = partition.m_branchBuf[i].m_bounds;
							BOUNDING		rect0 = Union(curRect, partition.m_cover[0]);
							BOUNDING		rect1 = Union(curRect, partition.m_cover[1]);
							F				growth0 = rect0.Volume() - partition.m_volume[0];
							F				growth1 = rect1.Volume() - partition.m_volume[1];
							F				diff = growth1 - growth0;

							if (diff >= 0)
								group = 0;
							else {
								group = 1;
								diff = -diff;
							}

							if (diff > biggestDiff) {
								biggestDiff = diff;
								chosen = i;
								betterGroup = group;
							}
							else if (diff == biggestDiff && partition.m_count[group] < partition.m_count[betterGroup]) {
								chosen = i;
								betterGroup = group;
							}
						}
					}
					Classify(chosen, betterGroup, partition);
				}

				// If one group too full, put remaining rects in the other
				if (partition.m_count[0] + partition.m_count[1] < MAXNODES + 1) {
					if (partition.m_count[0] >= MAXNODES + 1 - MINNODES)
						group = 1;
					else
						group = 0;

					for (int i=0; i < MAXNODES + 1; ++i) {
						if (Patitioning::UNASSIGNED == partition.m_partition[i])
							Classify(i, group, partition);
					}
				}

				ASSERT(partition.m_count[0] + partition.m_count[1] == MAXNODES + 1);
				ASSERT(partition.m_count[0] >= MINNODES && partition.m_count[1] >= MINNODES);
			}
		void LoadNodes(Node * first, Node * second, Patitioning & partition) {
				Node *		targetNodes[] = {first, second};
				for (int i=0; i < MAXNODES + 1; ++i) {
					ASSERT(partition.m_partition[i] == 0 || partition.m_partition[i] == 1);
					int		targetNodeIndex = partition.m_partition[i];
					AddBranch(partition.m_branchBuf[i], targetNodes[targetNodeIndex]);
				}
			}
		void PickSeeds(Patitioning & partition) {
				int		seed0, seed1;
				F		worst, waste;
				F		volume[MAXNODES+1];

				for (int i=0; i < MAXNODES + 1; ++i)
					volume[i] = partition.m_branchBuf[i].m_bounds.Volume();

				worst = -partition.m_total_volume - 1;
				for (int i = 0; i < MAXNODES; ++i) {
					for (int j = i+1; j < MAXNODES + 1; ++j) {
						BOUNDING	oneRect = Union(partition.m_branchBuf[i].m_bounds, partition.m_branchBuf[j].m_bounds);
						waste = oneRect.Volume() - volume[i] - volume[j];
						if (waste > worst) {
							worst = waste;
							seed0 = i;
							seed1 = j;
						}
					}
				}

				Classify(seed0, 0, partition);
				Classify(seed1, 1, partition);
			}
		void Classify(int index, int group, Patitioning & partition) {
				partition.m_partition[index] = group;

				// Calculate combined rect
				if (partition.m_count[group] == 0)
					partition.m_cover[group] = partition.m_branchBuf[index].m_bounds;
				else
					partition.m_cover[group] = Union(partition.m_branchBuf[index].m_bounds, partition.m_cover[group]);

				// Calculate volume of combined rect
				partition.m_volume[group] = partition.m_cover[group].Volume();

				++partition.m_count[group];
			}

		void ReInsert(Node * node, Node_List ** linker) {
				Node_List *		newListNode = AllocListNode();
				newListNode->m_node = node;
				newListNode->m_next = *linker;
				*linker = newListNode;
			}

		bool remove_item(BOUNDING & bounds, DATATYPE const & item, Node ** root) {
				Node_List * orphaned = null;

				if (!remove_recusrive(bounds, item, *root, &orphaned)) {
					// Found and deleted a data item
					// Reinsert any branches from eliminated nodes
					while (orphaned) {
						Node *		tempNode = orphaned->m_node;

						for (int i = 0; i < tempNode->m_count; ++i) {
							// TODO go over this code. should I use (tempNode->m_level - 1)?
							insert_item(tempNode->m_branch[i], root, tempNode->m_level);
						}
			
						Node_List *	remLNode = orphaned;
						orphaned = orphaned->m_next;
			
						FreeNode(remLNode->m_node);
						FreeListNode(remLNode);
					}
		
					// Check for redundant root (not leaf, 1 child) and eliminate TODO replace
					// if with while? In case there is a whole branch of redundant roots...
					if ((*root)->m_count == 1 && !(*root)->IsLeaf()) {
						Node * tempNode = (*root)->m_branch[0].m_child;
			
						ASSERT(tempNode);
						FreeNode(*root);
						*root = tempNode;
					}
					return false;
				}
				return true;
			}
		bool remove_recusrive(BOUNDING & bounds, DATATYPE const & item, Node * node, Node_List ** linker) {
				if (node->IsLeaf()) {
					for (int i = 0; i < node->m_count; ++i) {
						if (node->m_branch[i].m_data == item) {
							DisconnectBranch(node, i);
							return false;
						}
					}
					return true;
				}
				else {
					for (int i = 0; i < node->m_count; ++i) {
						if (bounds.Intersecting(node->m_branch[i].m_bounds)) {
							if (!remove_recusrive(bounds, item, node->m_branch[i].m_child, linker)) {
								if (node->m_branch[i].m_child->m_count >= MINNODES) {
									// child removed, just resize parent rect
									node->m_branch[i].m_bounds = node->m_branch[i].m_child->Volume();
								}
								else {
									// child removed, not enough entries in node, eliminate node
									ReInsert(node->m_branch[i].m_child, linker);
									DisconnectBranch(node, i); // Must return after this call as count has changed
								}
								return false;
							}
						}
					}
					return true;
				}
			}
		void delete_node_recusrive(Node * node) {		// probably should be Node destructor
				if (!node->IsLeaf())
					for (int i=0; i < node->m_count; ++i)
						delete_node_recusrive(node->m_branch[i].m_child);
				FreeNode(node); 
			}
		void Reset()								{ delete_node_recusrive(m_root); }

		bool Search(Node * node, BOUNDING const & bounds, int & found, Callback_Function callback, void * callback_data) {
				if (node->IsLeaf()) {
					// This is a leaf node
					for (int i=0; i < node->m_count; ++i) {
						if (bounds.Intersecting(node->m_branch[i].m_bounds)) {
							DATATYPE & id = node->m_branch[i].m_data;
							++found;

							// NOTE: There are different ways to return results.  Here's where to modify
							if (callback) {
								if (!callback(id, callback_data)) {
									return false; // Don't continue searching
								}
							}
						}
					}
				}
				else {
					// This is an internal node in the tree
					for (int i=0; i < node->m_count; ++i) {
						if (bounds.Intersecting(node->m_branch[i].m_bounds)) {
							if (!Search(node->m_branch[i].m_child, bounds, found, callback, callback_data)) {
								// The callback indicated to stop searching
								return false;
							}
						}
					}
				}
				return true; // Continue searching
			}

		int count_recursive(Node * node) {		// convert to member on Node
				if (node->IsLeaf())
					return node->m_count;
				int count = 0;
				for (int i = 0; i < node->m_count; ++i)
					count += count_recursive(node->m_branch[i].m_child, a_count);
				return count;
			}


	public:
		// constructor should probably have a memory pool to be used for all the internal node usage
		RStarTree(Memory_Pool * pool) : m_pool (pool) {
				ASSERT(MAXNODES > MINNODES);
				ASSERT(MINNODES > 0);
				// do we need to limit this?
				ASSERT(sizeof(DATATYPE) == sizeof(void *) || sizeof(DATATYPE) == sizeof(int));

				m_root = AllocNode();
				m_root->m_level = 0;
			}
		virtual ~RStarTree() {
				Reset();
			}
	
		void Insert (BOUNDING const & bounds, DATATYPE const & item) {
				Branch	branch;
				branch.m_data = item;
				branch.m_child = null;
				branch.m_bounds = bounds;
	
				insert_item(&branch, &m_root, 0);
			}
	
		void Remove (BOUNDING const & bounds, DATATYPE const & item) {
				remove_item (bounds, item, &m_root);
			}
	
		int Search (BOUNDING const & bounds, Callback_Function callback, void * callback_data) {
				int foundCount = 0;
				Search(m_root, &bounds, foundCount, callback, callback_data);

				return foundCount;
			}
	
		void RemoveAll() {
				Reset();
				m_root = AllocNode();
				m_root->m_level = 0;
			}

		int Count()									{ return count_recursive(m_root); }
};



// Method #0 for choosing a partition:
// As the seeds for the two groups, pick the two rects that would waste the
// most volume if covered by a single rectangle, i.e. evidently the worst pair
// to have in the same group.
// Of the remaining, one at a time is chosen to be put in one of the two groups.
// The one chosen is the one with the greatest difference in volume expansion
// depending on which group - the rect most strongly attracted to one group
// and repelled from the other.
// If one group gets too full (more would force other group to violate min
// fill requirement) then other group gets the rest.
// These last are the ones that can go in either group most easily.



#endif //RTREE_H

