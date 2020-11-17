/************************************************************************************
 * 
 * Name    : CParser
 * File    : CParser.h
 * Author  : Mark Reds <marco@markreds.it>
 * Date    : November 15, 2020
 * Version : 1.0.0
 * Notes   : Arduino string parser utility. Based on Luis Llamas Parser class.
 * 
 * Copyright (C) 2020 Marco Rossi (aka Mark Reds).  All right reserved.
 * 
 * This file is part of CParser.
 * 
 * CParser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * CParser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with CParser. If not, see <http://www.gnu.org/licenses/>.
 * 
 ************************************************************************************/

#ifndef _CParser_h_
#define _CParser_h_

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include "pins_arduino.h"
#endif

// Callbacks definitions
typedef void(*CParserCallback)();
typedef void(*CParserCallbackBool)(bool data);
typedef void(*CParserCallbackChar)(char data);
typedef void(*CParserCallbackByte)(byte data);
typedef void(*CParserCallbackUint8)(uint8_t data);
typedef void(*CParserCallbackUint16)(uint16_t data);
typedef void(*CParserCallbackUint32)(uint32_t data);
typedef void(*CParserCallbackInt8)(int8_t data);
typedef void(*CParserCallbackInt16)(int16_t data);
typedef void(*CParserCallbackInt32)(int32_t data);
typedef void(*CParserCallbackFloat)(float data);
typedef void(*CParserCallbackCharArray)(char *data, size_t length);
typedef void(*CParserCallbackString)(String &data);
typedef bool(*CParserCondition)();
typedef bool(*CParserCriterion)(byte data);

class CParser {
public:
	CParser();
	CParser(String &str);
	CParser(char *str);
	CParser(byte *buf, size_t len);
	virtual ~CParser();

	void begin(String &str);
	void begin(char *str);
	void begin(byte *buf, size_t len);

	char *currentItemPointer();
	char currentItem();
	void reset();
	inline bool isBufferOverflow();

	// Read methods
	bool readBool(CParserCallbackBool callback = nullptr);
	char readChar(CParserCallbackChar callback = nullptr);
	byte readByte(CParserCallbackByte callback = nullptr);
	int8_t readInt8(CParserCallbackInt8 callback = nullptr);
	int16_t readInt16(CParserCallbackInt16 callback = nullptr);
	int32_t readInt32(CParserCallbackInt32 callback = nullptr);
	uint8_t readUnsignedInt8(CParserCallbackUint8 callback = nullptr);
	uint16_t readUnsignedInt16(CParserCallbackUint16 callback = nullptr);
	uint32_t readUnsignedInt32(CParserCallbackUint32 callback = nullptr);

	float readFloat(CParserCallbackFloat callback = nullptr);
	float readUnsignedFloat(CParserCallbackFloat callback = nullptr);

	size_t readCharArray(char separator, CParserCallbackCharArray callback = nullptr);
	size_t readCharArray(CParserCriterion criterion, CParserCallbackCharArray callback = nullptr);
	size_t readCharArray(char separator, bool endIfNotFound, CParserCallbackCharArray callback = nullptr);
	size_t readCharArray(CParserCriterion criterion, bool endIfNotFound, CParserCallbackCharArray callback = nullptr);

	String readString(char separator, CParserCallbackString callback = nullptr);
	String readString(CParserCriterion criterion, CParserCallbackString callback = nullptr);
	String readString(char separator, bool endIfNotFound, CParserCallbackString callback = nullptr);
	String readString(CParserCriterion criterion, bool endIfNotFound, CParserCallbackString callback = nullptr);

	// compare methods
	bool compare(char token, CParserCallback callback = nullptr);
	bool compare(const char token[], CParserCallback callback = nullptr);
	bool compare(const char token[], size_t len, CParserCallback callback = nullptr);
	bool compare(String token, CParserCallback callback = nullptr);
	bool compare(CParserCriterion comparision, CParserCallback callback = nullptr);

	// search methods
	bool search(char token, CParserCallback callback = nullptr);
	bool search(char token[], CParserCallback callback = nullptr);
	bool search(char token[], size_t max_length, CParserCallback callback = nullptr);
	bool search(String token, CParserCallback callback = nullptr);
	bool search(CParserCriterion criterion, CParserCallback callback = nullptr);

	// Loop-if methods
	bool ifCurrentIs(char token, CParserCallback yesCallback = nullptr, CParserCallback noCallback = nullptr);
	bool ifCurrentIs(CParserCriterion criterion, CParserCallback yesCallback = nullptr, CParserCallback noCallback = nullptr);
	bool IfCurrentIsNot(char token, CParserCallback yesCallback = nullptr, CParserCallback noCallback = nullptr);
	bool IfCurrentIsNot(CParserCriterion criterion, CParserCallback yesCallback = nullptr, CParserCallback noCallback = nullptr);
	void doUntil(CParserCondition condition, CParserCallback callback = nullptr, CParserCallback finally = nullptr);
	void doWhile(CParserCondition condition, CParserCallback callback = nullptr, CParserCallback finally = nullptr);

	// skip methods
	void skip(size_t num_items);
	void skipWhile(char item);
	void skipWhile(CParserCriterion comparision);
	void skipUntil(char item);
	void skipUntil(CParserCriterion comparision);

	// Jump methods
	void jumpAfter(char item);
	void jumpAfter(CParserCriterion comparision);
	void jumpTo(char item);
	void jumpTo(CParserCriterion comparision);

	// Comparision static methods
	static bool isPrintable(byte item);
	static bool isAlfaNumeric(byte Item);
	static bool isNotDigit(byte Item);
	static bool isDigit(byte item);
	static bool isNumeric(byte item);
	static bool isLetter(byte item);
	static bool isNotLetter(byte item);
	static bool isUpperCaseLetter(byte item);
	static bool isLowerCaseLetter(byte item);
	static bool isSymbol(byte item);
	static bool isSeparator(byte item);
	static bool isNewLine(byte item);
	static bool isCarriageReturn(byte item);
	static bool isSeparatorOrNewLine(byte item);

private:
	byte *m_buf;
	size_t m_pos;
	size_t m_len;
	inline void next();
	inline bool compare(const char *str1, const char *str2, size_t n);

	template <class T_int> T_int readInteger();
	template <class T_uint> T_uint readUnsignedInteger();
};

#endif