// 2001-11-19 Benjamin Kosnik  <bkoz@redhat.com>

// Copyright (C) 2001 Free Software Foundation
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

// 22.2.2.2.1  num_put members

#include <locale>
#include <sstream>
#include <testsuite_hooks.h>

// XXX This test is not working for non-glibc locale models.
// { dg-do run { xfail *-*-* } }

#ifdef _GLIBCPP_USE_WCHAR_T
void test01()
{
  using namespace std;
  typedef ostreambuf_iterator<wchar_t> iterator_type;

  bool test = true;

  // basic construction
  locale loc_c = locale::classic();
  locale loc_hk("en_HK");
  locale loc_fr("fr_FR@euro");
  locale loc_de("de_DE");
  VERIFY( loc_c != loc_de );
  VERIFY( loc_hk != loc_fr );
  VERIFY( loc_hk != loc_de );
  VERIFY( loc_de != loc_fr );

  // cache the numpunct facets
  const numpunct<wchar_t>& numpunct_c = use_facet<numpunct<wchar_t> >(loc_c); 
  const numpunct<wchar_t>& numpunct_de = use_facet<numpunct<wchar_t> >(loc_de); 
  const numpunct<wchar_t>& numpunct_hk = use_facet<numpunct<wchar_t> >(loc_hk); 

  // sanity check the data is correct.
  const wstring empty;
  wstring result1;
  wstring result2;
  wchar_t c;

  bool b1 = true;
  bool b0 = false;
  long l1 = 2147483647;
  long l2 = -2147483647;
  unsigned long ul1 = 1294967294;
  unsigned long ul2 = 0;
  double d1 =  1.7976931348623157e+308;
  double d2 = 2.2250738585072014e-308;
  long double ld1 = 1.7976931348623157e+308;
  long double ld2 = 2.2250738585072014e-308;
  const void* cv = &ld1;

  // cache the num_put facet
  wostringstream oss;
  oss.imbue(loc_de);
  const num_put<wchar_t>& np = use_facet<num_put<wchar_t> >(oss.getloc()); 

  // bool, simple
  iterator_type os_it00 = oss.rdbuf();
  iterator_type os_it01 = np.put(os_it00, oss, '+', b1);
  result1 = oss.str();
  VERIFY( result1 == L"1" );
  //  VERIFY( os_it00 != os_it01 );

  oss.str(empty);
  np.put(oss.rdbuf(), oss, L'+', b0);
  result2 = oss.str();
  VERIFY( result2 == L"0" );

  // bool, more twisted examples
  oss.imbue(loc_c);
  oss.str(empty);
  oss.width(20);
  oss.setf(ios_base::right, ios_base::adjustfield);
  np.put(oss.rdbuf(), oss, L'+', b0);
  result1 = oss.str();
  VERIFY( result1 == L"+++++++++++++++++++0" );

  oss.str(empty);
  oss.width(20);
  oss.setf(ios_base::left, ios_base::adjustfield);
  oss.setf(ios_base::boolalpha);
  np.put(oss.rdbuf(), oss, L'+', b1);
  result2 = oss.str();
  VERIFY( result2 == L"true++++++++++++++++" );

  // long, in a locale that expects grouping
  oss.imbue(loc_hk);
  oss.str(empty);
  oss.clear();
  np.put(oss.rdbuf(), oss, L'+', l1);
  result1 = oss.str();
  VERIFY( result1 == L"2,147,483,647" );

  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.setf(ios_base::left, ios_base::adjustfield);
  np.put(oss.rdbuf(), oss, L'+', l2);
  result1 = oss.str();
  VERIFY( result1 == L"-2,147,483,647++++++" );

  // unsigned long, in a locale that does not group
  oss.imbue(loc_c);
  oss.str(empty);
  oss.clear();
  np.put(oss.rdbuf(), oss, L'+', ul1);
  result1 = oss.str();
  VERIFY( result1 == L"1294967294" );

  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.setf(ios_base::left, ios_base::adjustfield);
  np.put(oss.rdbuf(), oss, L'+', ul2);
  result1 = oss.str();
  VERIFY( result1 == L"0+++++++++++++++++++" );

  // ... and one that does
  oss.imbue(loc_de);
  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.setf(ios_base::left, ios_base::adjustfield);
  np.put(oss.rdbuf(), oss, L'+', ul1);
  result1 = oss.str();
  VERIFY( result1 == L"1.294.967.294+++++++" );

  // double
  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.setf(ios_base::left, ios_base::adjustfield);
  np.put(oss.rdbuf(), oss, L'+', d1);
  result1 = oss.str();
  VERIFY( result1 == L"1,79769e+308++++++++" );

  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.setf(ios_base::right, ios_base::adjustfield);
  np.put(oss.rdbuf(), oss, L'+', d2);
  result1 = oss.str();
  VERIFY( result1 == L"++++++++2,22507e-308" );

  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.setf(ios_base::right, ios_base::adjustfield);
  oss.setf(ios_base::scientific, ios_base::floatfield);
  np.put(oss.rdbuf(), oss, L'+', d2);
  result2 = oss.str();
  VERIFY( result2 == L"+++++++2,225074e-308" );

  oss.str(empty);
  oss.clear();
  oss.width(20);
  oss.precision(10);
  oss.setf(ios_base::right, ios_base::adjustfield);
  oss.setf(ios_base::scientific, ios_base::floatfield);
  oss.setf(ios_base::uppercase);
  np.put(oss.rdbuf(), oss, L'+', d2);
  result1 = oss.str();
  VERIFY( result1 == L"+++2,2250738585E-308" );

  // long double
  oss.str(empty);
  oss.clear();
  np.put(oss.rdbuf(), oss, L'+', ld1);
  result1 = oss.str();
  VERIFY( result1 == L"1,7976931349E+308" );

  oss.str(empty);
  oss.clear();
  oss.precision(0);
  oss.setf(ios_base::fixed, ios_base::floatfield);
  np.put(oss.rdbuf(), oss, L'+', ld2);
  result1 = oss.str();
  VERIFY( result1 == L"0" );

  // const void
  oss.str(empty);
  oss.clear();
  np.put(oss.rdbuf(), oss, L'+', cv);
  result1 = oss.str();
  // No grouping characters.
  VERIFY( !char_traits<wchar_t>::find(result1.c_str(), 
				   numpunct_de.decimal_point(), 
				   result1.size()) );
  // Should contain an 'x'.
  VERIFY( !char_traits<wchar_t>::find(result1.c_str(), L'x', result1.size()) );

#ifdef _GLIBCPP_USE_LONG_LONG
  long long ll1 = 9223372036854775807;
  long long ll2 = -9223372036854775807;

  oss.str(empty);
  oss.clear();
  np.put(oss.rdbuf(), oss, '+', ll1);
  result1 = oss.str();
  VERIFY( result1 == L"9.223.372.036.854.775.807" );
#endif
}
#endif

int main()
{
#ifdef _GLIBCPP_USE_WCHAR_T
  test01();
#endif
  return 0;
}


// Diana D. Brooks, former chief executive of Sotheby's
// art-thief extraordinaire
