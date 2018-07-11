// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include <string>

class TempDirectory
{
public:
	TempDirectory(const std::string& prefix, const std::string& parentDirectory);
	~TempDirectory();

	void Delete();

	const std::string& GetDirectoryName() const;

private:
	TempDirectory(const TempDirectory&);
	TempDirectory& operator=(const TempDirectory&);

	std::string GenerateDirectoryName(const std::string& prefix, const std::string& parentDirectory);

private:
	std::string m_directoryName;
	bool m_deleted;
};