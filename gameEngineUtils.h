#pragma once
#ifndef GAMEENGINEUTILS_H
#define GAMEENGINEUTILS_H
#pragma warning(disable : 4996)



#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <algorithm>
#include <vector>


#include "document.h"
#include "filereadstream.h"

class gameEngineUtils
{

public:
	static void ReadJsonFile(const std::string& path, rapidjson::Document& out_document)
	{
		FILE* file_pointer = nullptr;
#ifdef _WIN32
		fopen_s(&file_pointer, path.c_str(), "rb");
#else
		file_pointer = fopen(path.c_str(), "rb");
#endif
		char buffer[65536];
		rapidjson::FileReadStream stream(file_pointer, buffer, sizeof(buffer));
		out_document.ParseStream(stream);
		std::fclose(file_pointer);

		if (out_document.HasParseError()) {
			rapidjson::ParseErrorCode errorCode = out_document.GetParseError();
			std::cout << "error parsing json at [" << path << "]" << std::endl;
			exit(0);
		}
	}
	static void HandlePrint_PV(std::string myString)
	{
		std::cout << myString + "\n";
	}

	static void HandlePrint_PR(std::string& myString)
	{
		std::cout << myString + "\n";
	}
	static void HandlePrint_PV(std::string myString, bool skipEndl)
	{
		// use this for end game stuff
		if (skipEndl)
		{
			std::cout << myString;
		}
		else
		{
			std::cout << myString + "\n";
		}
	}

	static void HandlePrint_PR(std::string& myString, bool skipEndl)
	{
		// use this for end game stuff
		if (skipEndl)
		{
			std::cout << myString;
		}
		else
		{
			std::cout << myString + "\n";
		}
	}


	static std::string obtain_word_after_phrase(const std::string& input, const std::string& phrase) 
	{
		// find position of the phrase in the string
		size_t pos = input.find(phrase);

		// if not found return empty
		if (pos == std::string::npos) return "";

		// find the starting point of the next word
		pos += phrase.length();
		while (pos < input.size() && std::isspace(input[pos])) 
		{
			pos++;
		}

		// if were at the end, empty string
		if (pos == input.size()) return "";

		// find the end position of the word (ubtil a space or end of string)
		size_t endPos = pos;
		while (endPos < input.size() && !std::isspace(input[endPos])) 
		{
			++endPos;
		}

		return input.substr(pos, endPos - pos);
	}

	/*
	static void Actor_Sort_ID( std::vector< Actor_C* > &myVec ) 
	{
		std::sort(myVec.begin(), myVec.end(), [](Actor_C* first, Actor_C* second)
			{
				return first->ID < second->ID;
			});
	}
	static void Actor_Sort_Render(std::vector< Actor_C* >& myVec)
	{
		std::sort(myVec.begin(), myVec.end(), [](Actor_C* first, Actor_C* second)
			{
				float first_value = first->position.y;
				float second_value = second->position.y;

				if (first->render_order.has_value()) 
				{
					first_value = first->render_order.value();
				}
				if (second->render_order.has_value())
				{
					second_value = second->render_order.value();
				}

				if (first->position.y == second->position.y) 
				{
					return first->ID < second->ID;
				}
				
				return first_value < second_value;

			});
	}
	*/


	uint64_t create_composite_key(float x, float y)
	{

		// compresses x and y into 32 bits each, places x into a 64 bit int and shifts it, ors them together to make a single 64 bit number, taken from 498
		uint32_t ux = static_cast<uint32_t>(x); // / largest_collider_width);
		uint32_t uy = static_cast<uint32_t>(y); // / largest_collider_height);

		uint64_t result = static_cast<uint64_t>(ux);

		result = result << 32;

		result = result | static_cast<uint64_t>(uy);

		return result;
	}

	static bool isOverlapping(const float& box_width_A, const float& box_height_A, const float& box_X_A, const float& box_Y_A, const float& box_width_B, const float& box_height_B, const float& box_X_B, const float& box_Y_B, bool is_player) 
	{
		if (box_width_A == 0 || box_width_B == 0 || box_height_A == 0 || box_height_B == 0) 
		{
			return false;
		}
		// check if right side of A is past left side of B and left side of A is not pass right side of B
		if (((box_X_A + box_width_A) > box_X_B) && !(box_X_A > (box_X_B + box_width_B)) && (box_Y_B < (box_Y_A + box_height_A)) && ((box_Y_B + box_height_B) > box_Y_A))
		{
			return true;
		}
		return false;
		// check if top of A is above bottom of B and bottom of A is not passed top of B
		
	
	
	}


	static void CheckIfUint64_TIsAlreadyPresent(uint64_t key_temp, std::vector<uint64_t>& relaventKeys)
	{
		bool containsKey = false;
		for (uint64_t key : relaventKeys)
		{
			if (key == key_temp)
			{
				containsKey = true;
				break;
			}
		}
		if (!containsKey)
		{
			relaventKeys.push_back(key_temp);
		}
	}

};


#endif // !GAMEENGINEUTILS_H