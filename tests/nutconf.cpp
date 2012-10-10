/* example - CppUnit unit test example

   Copyright (C)
	2012	Emilien Kia <emilienkia-guest@alioth.debian.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#include <cppunit/extensions/HelperMacros.h>

// Define to desactivate protection of parsing tool members:
#define UNITEST_MODE 1

#include "nutconf.h"
using namespace nut;

#include <string>
using namespace std;

class NutConfTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( NutConfTest );
    CPPUNIT_TEST( testParseCHARS );
    CPPUNIT_TEST( testParseSTRCHARS );
    CPPUNIT_TEST( testPasreToken );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testParseCHARS();
  void testParseSTRCHARS();
  void testPasreToken();
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( NutConfTest );


void NutConfTest::setUp()
{
}


void NutConfTest::tearDown()
{
}


void NutConfTest::testParseCHARS()
{
    {
        NutConfigParser parse("Bonjour monde!");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Cannot find first string 'Bonjour'", string("Bonjour"), parse.parseCHARS());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Cannot get a character ''", ' ', parse.get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Cannot find second string 'monde!'", string("monde!"), parse.parseCHARS());
    }

    {
        NutConfigParser parse("To\\ to");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Cannot find escaped string 'To to'", string("To to"), parse.parseCHARS());
    }

    {
        NutConfigParser parse("To\"to");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Cannot find escaped string 'To'", string("To"), parse.parseCHARS());
    }

    {
        NutConfigParser parse("To\\\"to");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Cannot find escaped string 'To\"to'", string("To\"to"), parse.parseCHARS());
    }

}


void NutConfTest::testParseSTRCHARS()
{
    {
        NutConfigParser parse("Bonjour\"monde!\"");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Cannot find first string 'Bonjour'", string("Bonjour"), parse.parseSTRCHARS());
        parse.get();
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Cannot find second string 'monde!'", string("monde!"), parse.parseSTRCHARS());
    }

    {
        NutConfigParser parse("To to");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Cannot find spaced string 'To tue de l’appareil qui se serait malencontreuo'", string("To to"), parse.parseSTRCHARS());
    }

    {
        NutConfigParser parse("To\\\"to");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Cannot find quoted-escaped string 'To\"to'", string("To\"to"), parse.parseSTRCHARS());
    }
}

void NutConfTest::testPasreToken()
{
    static const char* src =
        "Bonjour monde\n"
        "[ceci]# Plouf\n"
        "\n"
        "titi = \"tata toto\"";
    NutConfigParser parse(src);

//    NutConfigParser::Token tok = parse.parseToken();
//    std::cout << "token = " << tok.type << " - " << tok.str << std::endl;

    CPPUNIT_ASSERT_MESSAGE("Cannot find 1st token 'Bonjour'", parse.parseToken() == NutConfigParser::Token(NutConfigParser::Token::TOKEN_STRING, "Bonjour"));
    CPPUNIT_ASSERT_MESSAGE("Cannot find 2nd token 'monde'", parse.parseToken() == NutConfigParser::Token(NutConfigParser::Token::TOKEN_STRING, "monde"));
    CPPUNIT_ASSERT_MESSAGE("Cannot find 3rd token '['", parse.parseToken() == NutConfigParser::Token(NutConfigParser::Token::TOKEN_BRACKET_OPEN, "["));
    CPPUNIT_ASSERT_MESSAGE("Cannot find 4th token 'ceci'", parse.parseToken() == NutConfigParser::Token(NutConfigParser::Token::TOKEN_STRING, "ceci"));
    CPPUNIT_ASSERT_MESSAGE("Cannot find 5th token ']'", parse.parseToken() == NutConfigParser::Token(NutConfigParser::Token::TOKEN_BRACKET_CLOSE, "]"));
    CPPUNIT_ASSERT_MESSAGE("Cannot find 6th token ' Plouf'", parse.parseToken() == NutConfigParser::Token(NutConfigParser::Token::TOKEN_COMMENT, " Plouf"));
    CPPUNIT_ASSERT_MESSAGE("Cannot find 7th token '\n'", parse.parseToken() == NutConfigParser::Token(NutConfigParser::Token::TOKEN_EOL, "\n"));
    CPPUNIT_ASSERT_MESSAGE("Cannot find 8th token 'titi'", parse.parseToken() == NutConfigParser::Token(NutConfigParser::Token::TOKEN_STRING, "titi"));
    CPPUNIT_ASSERT_MESSAGE("Cannot find 9th token '='", parse.parseToken() == NutConfigParser::Token(NutConfigParser::Token::TOKEN_EQUAL, "="));
    CPPUNIT_ASSERT_MESSAGE("Cannot find 10th token 'tata toto'", parse.parseToken() == NutConfigParser::Token(NutConfigParser::Token::TOKEN_QUOTED_STRING, "tata toto"));

}