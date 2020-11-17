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

#include "CParser.h"

CParser::CParser() { }

CParser::CParser(String &str) {
	begin(str);
}

CParser::CParser(char *str) {
	begin(str);
}

CParser::CParser(byte *buf, size_t len) {
	begin(buf, len);
}

CParser::~CParser() { }

void CParser::begin(String &str) {
	begin((char *)str.c_str());
}

void CParser::begin(char *str) {
	begin((byte *)str, strlen(str));
}

void CParser::begin(byte *buf, size_t len) {
	m_buf = buf;
	m_len = len;
	m_pos = 0;
}

char *CParser::currentItemPointer() {
	return (char*)m_buf + m_pos;
}

char CParser::currentItem() {
	if (isBufferOverflow()) {
		return '\0';
	}
	return (char)m_buf[m_pos];
}

void CParser::reset() {
	m_pos = 0;
}

inline bool CParser::isBufferOverflow() {
	return m_pos >= m_len;
}

// Read methods
bool CParser::readBool(CParserCallbackBool callback) {
	char rst = currentItem();
	
	if (rst == '1') {
		if (callback != nullptr) {
			callback(true);
		}
		return true;
	}

	if (rst == '0') {
		if (callback != nullptr) {
			callback(false);
		}
		return false;
	}

	return false;
}

char CParser::readChar(CParserCallbackChar callback) {
	char rst = currentItem();
	next();
	return rst;
}

byte CParser::readByte(CParserCallbackByte callback) {
	byte rst = currentItem();
	next();
	return rst;
}

int8_t CParser::readInt8(CParserCallbackInt8 callback) {
	bool valid = CParser::isNumeric(currentItem());
	if (!valid) {
		return 0;
	}

	int8_t data = readInteger<int8_t>();
	if (callback != nullptr) {
		callback(data);
	}

	return data;
}

int16_t CParser::readInt16(CParserCallbackInt16 callback) {
	bool valid = CParser::isNumeric(currentItem());
	if (!valid) {
		return 0;
	}

	int16_t data = readInteger<int16_t>();
	
	if (callback != nullptr) {
		callback(data);
	}

	return data;
}

int32_t CParser::readInt32(CParserCallbackInt32 callback) {
	bool valid = CParser::isNumeric(currentItem());
	if (!valid) {
		return 0;
	}

	int32_t data = readInteger<int32_t>();
	
	if (callback != nullptr) {
		callback(data);
	}

	return data;
}

uint8_t CParser::readUnsignedInt8(CParserCallbackUint8 callback) {
	bool valid = CParser::isDigit(currentItem());
	if (!valid) {
		return 0;
	}

	uint8_t data = readUnsignedInteger<uint8_t>();
	
	if (callback != nullptr) {
		callback(data);
	}

	return data;
}

uint16_t CParser::readUnsignedInt16(CParserCallbackUint16 callback) {
	bool valid = CParser::isDigit(currentItem());
	if (!valid) {
		return 0;
	}

	uint16_t data = readUnsignedInteger<uint16_t>();
	
	if (callback != nullptr) {
		callback(data);
	}
	
	return data;
}

uint32_t CParser::readUnsignedInt32(CParserCallbackUint32 callback) {
	bool valid = CParser::isDigit(currentItem());
	if (!valid) {
		return 0;
	}

	uint32_t data = readUnsignedInteger<uint32_t>();
	
	if (callback != nullptr) {
		callback(data);
	}

	return data;
}

float CParser::readFloat(CParserCallbackFloat callback) {
	bool valid = CParser::isNumeric(currentItem());
	if (!valid) return 0;

	float data = 0;
	int dataReal = 0;
	int dataDecimal = 0;
	int dataPow = 1;
	bool isDecimalStage = false;

	bool isNegative = false;
	if (m_buf[m_pos] == '-') {
		isNegative = true;
		m_pos++;
	}

	while (m_pos < m_len) {
		char incomingChar = m_buf[m_pos];

		if (incomingChar == '.' || incomingChar == ',') {
			isDecimalStage = true;
		} else if (incomingChar >= '0' && incomingChar <= '9') {
			if (isDecimalStage == false) {
				dataReal = (dataReal * 10) + (incomingChar - '0');
			} else {
				dataDecimal = (dataDecimal * 10) + (incomingChar - '0');
				dataPow *= 10;
			}
		} else {
			break;
		}

		m_pos++;
	}

	data = (float)dataReal + (float)dataDecimal / dataPow;
	data = isNegative ? -data : data;

	if (callback != nullptr) {
		callback(data);
	}
	return data;
}

float CParser::readUnsignedFloat(CParserCallbackFloat callback) {
	bool valid = CParser::isNumeric(currentItem());
	if (!valid) return 0;

	float data = 0;
	int dataReal = 0;
	int dataDecimal = 0;
	int dataPow = 1;
	bool isDecimalStage = false;

	while (m_pos < m_len) {
		char incomingChar = m_buf[m_pos];

		if (incomingChar == '.' || incomingChar == ',')
			isDecimalStage = true;

		else if (incomingChar >= '0' && incomingChar <= '9') {
			if (isDecimalStage == false) {
				dataReal = (dataReal * 10) + (incomingChar - '0');
			} else {
				dataDecimal = (dataDecimal * 10) + (incomingChar - '0');
				dataPow *= 10;
			}
		} else {
			break;
		}

		m_pos++;
	}

	data = (float)dataReal + (float)dataDecimal / dataPow;

	if (callback != nullptr) {
		callback(data);
	}

	return data;
}

size_t CParser::readCharArray(char separator, CParserCallbackCharArray callback) {
	return readCharArray(separator, true, callback);
}

size_t CParser::readCharArray(CParserCriterion criterion, CParserCallbackCharArray callback) {
	return readCharArray(criterion, true, callback);
}

size_t CParser::readCharArray(char separator, bool endIfNotFound, CParserCallbackCharArray callback) {
	size_t length = 0;
	bool found = false;

	char *start = currentItemPointer();
	while (m_pos + length < m_len) {
		if (m_buf[m_pos + length] == separator) {
			found = true;
			break;
		}
		length++;
	}
	m_pos += length + (found ? 1 : 0);

	if ((endIfNotFound || (!endIfNotFound && found))) {
		if (callback != nullptr) callback(start, length);
	}

	return length;
}

size_t CParser::readCharArray(CParserCriterion criterion, bool endIfNotFound, CParserCallbackCharArray callback) {
	size_t length = 0;
	bool found = false;

	char *start = currentItemPointer();
	while (m_pos + length < m_len) {
		if (criterion(m_buf[m_pos + length])) {
			found = true;
			break;
		}
		length++;
	}
	m_pos += length + (found ? 1 : 0);

	if ((endIfNotFound || (!endIfNotFound && found))) {
		if (callback != nullptr) {
			callback(start, length);
		}
	}

	return length;
}

String CParser::readString(char separator, CParserCallbackString callback) {
	return readString(separator, true, callback);
}

String CParser::readString(CParserCriterion criterion, CParserCallbackString callback) {
	return readString(criterion, true, callback);
}

String CParser::readString(char separator, bool endIfNotFound, CParserCallbackString callback) {
	String rst;
	size_t length = 0;
	bool found = false;

	char *start = currentItemPointer();
	while (m_pos + length < m_len) {
		if (m_buf[m_pos + length] == separator) {
			found = true;
			break;
		}
		length++;
	}
	m_pos += length;//+ (found ? 1 : 0);

	if ((endIfNotFound || (!endIfNotFound && found))) {
		rst.reserve(length);
		for (size_t i = 0; i < length; i++) {
			rst.concat(start[i]);
		}

		if (callback != nullptr) {
			callback(rst);
		}
	}

	return rst;
}

String CParser::readString(CParserCriterion criterion, bool endIfNotFound, CParserCallbackString callback) {
	String rst;
	size_t length = 0;
	bool found = false;

	char *start = currentItemPointer();
	while (m_pos + length < m_len) {
		if (criterion(m_buf[m_pos + length])) {
			found = true;
			break;
		}
		length++;
	}

	m_pos += length;

	if ((endIfNotFound || (!endIfNotFound && found))) {
		rst.reserve(length);
		for (size_t i = 0; i < length; i++) {
			rst.concat(start[i]);
		}

		if (callback != nullptr) {
			callback(rst);
		}
	}

	return rst;
}


// compare methods
bool CParser::compare(char token, CParserCallback callback) {
	if (m_pos >= m_len) {
		return false;
	}

	if (currentItem() == token) {
		if (callback != nullptr) {
			callback();
		}
		return true;
	}
	
	return false;
}

bool CParser::compare(const char token[], CParserCallback callback) {
	return compare(token, strlen(token), callback);
}

bool CParser::compare(const char token[], size_t len, CParserCallback callback) {
	if (m_pos >= m_len) {
		return false;
	}

	bool found = false;
	if (compare((char *)m_buf, token, len)) {
		if (callback != nullptr) {
			callback();
		}
		found = true;
	}
	return found;
}

bool CParser::compare(String token, CParserCallback callback) {
	return compare((char*)token.c_str(), token.length(), callback);
}

bool CParser::compare(CParserCriterion comparision, CParserCallback callback) {
	if (m_pos >= m_len) {
		return false;
	}

	bool found = false;
	if (!comparision(currentItem())) {
		if (callback != nullptr) {
			callback();
		}
		found = true;
	}
	return found;
}

// search methods
bool CParser::search(char token, CParserCallback callback) {
	for (size_t index = m_pos; index < m_len; index++) {
		if (m_buf[index] == token) {
			if (callback != nullptr) {
				callback();
			}
			return true;
		}
	}
	return false;
}

bool CParser::search(char token[], CParserCallback callback) {
	return search(token, strlen(token), callback);
}

bool CParser::search(char token[], size_t max_length, CParserCallback callback) {
	for (size_t index = m_pos; index < m_len; index++) {
		if (compare((char*)m_buf, token, max_length)) {
			if (callback != nullptr) {
				callback();
			}
			return true;
		}
	}
	return false;
}

bool CParser::search(String token, CParserCallback callback) {
	return search((char*)token.c_str(), token.length(), callback);
}

bool CParser::search(CParserCriterion comparision, CParserCallback callback) {
	for (size_t index = m_pos; index < m_len; index++) {
		if (comparision(m_buf[index])) {
			if (callback != nullptr) {
				callback();
			}
			return true;
		}
	}
	return false;
}


// Loop-if methods
bool CParser::ifCurrentIs(char token, CParserCallback yesCallback, CParserCallback noCallback) {
	if (currentItem() == token) {
		if (yesCallback != nullptr) {
			yesCallback();
		}
		return true;
	}
	if (noCallback != nullptr) {
		noCallback();
	}
	return false;
}

bool CParser::ifCurrentIs(CParserCriterion criterion, CParserCallback yesCallback, CParserCallback noCallback) {
	if (criterion(currentItem())) {
		if (yesCallback != nullptr) {
			yesCallback();
		}
		return true;
	}
	if (noCallback != nullptr) {
		noCallback();
	}
	return false;
}

bool CParser::IfCurrentIsNot(char token, CParserCallback yesCallback, CParserCallback noCallback) {
	if (currentItem() != token) {
		if (yesCallback != nullptr) {
			yesCallback();
		}
		return true;
	}
	if (noCallback != nullptr) {
		noCallback();
	}
	return false;
}

bool CParser::IfCurrentIsNot(CParserCriterion criterion, CParserCallback yesCallback, CParserCallback noCallback) {
	if (!criterion(currentItem())) {
		if (yesCallback != nullptr) {
			yesCallback();
		}
		return true;
	}
	if (noCallback != nullptr) {
		noCallback();
	}
	return false;
}

void CParser::doUntil(CParserCondition condition, CParserCallback callback, CParserCallback finally) {
	while (!condition()) {
		if (callback != nullptr) {
			callback();
		}
	}
	if (finally != nullptr) {
		finally();
	}
}

void CParser::doWhile(CParserCondition condition, CParserCallback callback, CParserCallback finally) {
	while (condition()) {
		if (callback != nullptr) {
			callback();
		}
	}
	if (finally != nullptr) {
		finally();
	}
}


// skip methods
void CParser::skip(size_t num_items) {
	m_pos += num_items;
	if (isBufferOverflow()) {
		m_pos = m_len - 1;
	}
}

void CParser::skipWhile(char item) {
	while (!isBufferOverflow() && currentItem() == item) {
		m_pos++;
	}
}

void CParser::skipWhile(CParserCriterion comparision) {
	while (!isBufferOverflow() && comparision(currentItem())) {
		m_pos++;
	}
}

void CParser::skipUntil(char item) {
	while (!isBufferOverflow() && (currentItem() != item)) {
		m_pos++;
	}
}

void CParser::skipUntil(CParserCriterion comparision) {
	while (!isBufferOverflow() && !comparision(currentItem())) {
		m_pos++;
	}
}

// Jump methods
void CParser::jumpAfter(char item) {
	size_t index = m_pos;
	while (index < m_len) {
		if (m_buf[index] == item) {
			m_pos = index;
			next();
			return;
		}
		index++;
	}
}

void CParser::jumpAfter(CParserCriterion comparision) {
	size_t index = m_pos;
	while (index < m_len) {
		if (comparision(m_buf[index])) {
			m_pos = index;
			next();
			return;
		}
		index++;
	}
}

void CParser::jumpTo(char item) {
	size_t index = m_pos;
	while (index < m_len) {
		if (m_buf[index] == item) {
			m_pos = index;
			return;
		}
		index++;
	}
}

void CParser::jumpTo(CParserCriterion comparision) {
	size_t index = m_pos;
	while (index < m_len) {
		if (comparision(m_buf[index])) {
			m_pos = index;
			return;
		}
		index++;
	}
}

//Static methods
bool CParser::isPrintable(byte item) {
	return item >= 32 && item < 129;
}

bool CParser::isAlfaNumeric(byte item) {
	return isLetter(item) || isDigit(item);
}

bool CParser::isNotDigit(byte item) {
	return !isDigit(item);
}

bool CParser::isDigit(byte item) {
	return item >= '0' && item <= '9';
}

bool CParser::isNumeric(byte item) {
	return isDigit(item) || item == '.' || item == ',' || item == '-';
}

bool CParser::isLetter(byte item) {
	return isUpperCaseLetter(item) || isLowerCaseLetter(item);
}

bool CParser::isNotLetter(byte item) {
	return !isLetter(item);
}

bool CParser::isUpperCaseLetter(byte item) {
	return item >= 'A' && item <= 'Z';
}

bool CParser::isLowerCaseLetter(byte item) {
	return item >= 'a' && item <= 'z';
}

bool CParser::isSeparator(byte item) {
	return item = '|' || item == '.' || item == ',' || item == ';' || item == ' ' ||
		item == '_' || item == '-' || item == '#' || item == '?' || item == '\0';
}

bool CParser::isSymbol(byte item) {
	return isPrintable(item) && !isDigit(item) && !isLetter(item);
}

bool CParser::isNewLine(byte item) {
	return item == '\n';
}

bool CParser::isCarriageReturn(byte item) {
	return item == '\r';
}

bool CParser::isSeparatorOrNewLine(byte item) {
	return isSeparator(item) || isNewLine(item);
}

// Private methods
inline void CParser::next() {
	if (++m_pos >= m_len) {
		m_pos = m_len;
	}
}

inline bool CParser::compare(const char *str1, const char *str2, size_t n) {
	bool equals = true;

	size_t newIndex = m_pos;
	for (size_t index = 0; index < n; index++) {
		if (newIndex >= m_len) {
			equals = false;
			break;
		}

		if (str1[newIndex] != str2[index]) {
			equals = false;
			break;
		}

		newIndex++;
	}

	if (equals) {
		m_pos = newIndex;
	}

	return equals;
}

template<class T_int> T_int CParser::readInteger() {
	T_int rst = 0;

	bool isNegative = false;
	if (m_buf[m_pos] == '-') {
		isNegative = true;
		m_pos++;
	}

	while (m_pos < m_len) {
		char incomingChar = m_buf[m_pos];
		if (incomingChar >= '0' && incomingChar <= '9') {
			rst = (rst * 10) + (incomingChar - '0');
		} else {
			break;
		}
		m_pos++;
	}
	return isNegative ? -rst : rst;
}

template<class T_uint> inline T_uint CParser::readUnsignedInteger() {
	T_uint rst = 0;

	while (m_pos < m_len) {
		char incomingChar = m_buf[m_pos];

		if (incomingChar >= '0' && incomingChar <= '9') {
			rst = (rst * 10) + (incomingChar - '0');
		} else {
			break;
		}
		m_pos++;
	}
	return rst;
}