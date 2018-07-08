/* -----------------------------------------------------------------------------
 * wchar_t.i
 *
 * Typemaps for const wchar_t *
 * 
 * Based on $swig\Lib\java\std_wstring.i
 * ----------------------------------------------------------------------------- */

%include "std_wstring.i"
%apply const std::wstring & {const wchar_t *};

%typemap(in) const wchar_t *
%{if(!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null wchar_t");
    return $null;
  }
  const jchar *$1_pstr = jenv->GetStringChars($input, 0);
  if (!$1_pstr) return $null;
  jsize $1_len = jenv->GetStringLength($input);
  std::wstring $1_str;
  if ($1_len) {
    $1_str.reserve($1_len);
    for (jsize i = 0; i < $1_len; ++i) {
      $1_str.push_back((wchar_t)$1_pstr[i]);
    }
  }
  $1 = ($1_ltype) $1_str.data();
  jenv->ReleaseStringChars($input, $1_pstr);
 %}

%typemap(directorout,warning=SWIGWARN_TYPEMAP_THREAD_UNSAFE_MSG) const wchar_t *
%{if(!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null wchar_t");
    return $null;
  }
  const jchar *$1_pstr = jenv->GetStringChars($input, 0);
  if (!$1_pstr) return $null;
  jsize $1_len = jenv->GetStringLength($input);
  /* possible thread/reentrant code problem */
  static std::wstring $1_str;
  if ($1_len) {
    $1_str.reserve($1_len);
    for (jsize i = 0; i < $1_len; ++i) {
      $1_str.push_back((wchar_t)$1_pstr[i]);
    }
  }
  $result = ($1_ltype) $1_str.data();
  jenv->ReleaseStringChars($input, $1_pstr); %}

%typemap(directorin,descriptor="Ljava/lang/String;") const wchar_t * {
  jsize $1_len = std::wcslen($1);
  jchar *conv_buf = new jchar[$1_len];
  for (jsize i = 0; i < $1_len; ++i) {
    conv_buf[i] = (jchar) $1[i];
  }
  $input = jenv->NewString(conv_buf, $1_len);
  delete [] conv_buf;
}

%typemap(out) const wchar_t *
%{jsize $1_len = std::wcslen($1);
  jchar *conv_buf = new jchar[$1_len];
  for (jsize i = 0; i < $1_len; ++i) {
    conv_buf[i] = (jchar) $1[i];
  }
  $result = jenv->NewString(conv_buf, $1_len);
  delete [] conv_buf; %}

