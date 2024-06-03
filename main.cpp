#define CURL_STATICLIB

#include "curl.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <vector>


using json = nlohmann::json;


//---VARIABLES---

//main, not changeable api url
const std::string apiUrlMainBody = "https://pokeapi.co/api/v2/";

//url with the endpoint
std::string apiUrlEndpoint;

//the api url, apiUrlMainBody and apiUrlEndpoint combined
std::string apiUrl;


//---FUNCTIONS---
size_t writeCallback(void* content, size_t size, size_t nmemb, std::string* s);

std::string returnRequestResult(const std::string& apiUrl);

std::vector<std::pair<std::string, std::string>> extractResponseData(const std::string& apiResponse);


void printPokemonInfo(const std::map<std::string, std::string>& map);
void inputMenu();
void mainMenu();


int main()
{

	mainMenu();
	return 0;
}


size_t writeCallback(void* content, size_t size, size_t nmemb, std::string* s)
{
	size_t totalSize = size * nmemb;
	s->append(static_cast<char*>(content), totalSize);
	return totalSize;
}

std::string returnRequestResult(const std::string& apiUrl)
{
	CURL* curl;
	CURLcode result;
	std::string response;

	curl = curl_easy_init();

	if (curl == NULL)
	{
		std::cout << "Error: couldn't initialize curl";
		return " ";
	}

	curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	result = curl_easy_perform(curl);

	if (result != CURLE_OK)
	{
		std::cout << "Error:" << curl_easy_strerror(result);
		curl_easy_cleanup(curl);
		return " ";
	}

	curl_easy_cleanup(curl);
	return response;
}



std::vector<std::pair<std::string, std::string>> extractResponseData(const std::string& apiResponse)
{
	std::vector<std::pair<std::string, std::string>> data;

	json jsonResponse = json::parse(apiResponse);

	//pokemon's ability
	if (jsonResponse.contains("is_main_series"))
	{
		std::string abilityName = jsonResponse["name"];
		data.push_back({ "ability" , abilityName });
		
		for (const auto& i : jsonResponse["effect_entries"])
		{
			if (i["language"]["name"] == "en")
			{
				std::string effect = i["effect"];
				std::string shortEffect = i["short_effect"];

				data.push_back({ "effect", effect });
				data.push_back({ "short effect", shortEffect });
			}
		}
	}
	
	//pokemon's name
	else if (jsonResponse.contains("species"))
	{
		std::string speciesName = jsonResponse["species"]["name"];
		int baseExperience = jsonResponse["base_experience"];
		int height = jsonResponse["height"];
		int weight = jsonResponse["weight"];

		data.push_back({ "base experience", std::to_string(baseExperience) });
		data.push_back({ "height", std::to_string(height) });
		data.push_back({ "weight", std::to_string(weight) });
		data.push_back({ "species", speciesName });

		for (const auto& i : jsonResponse["abilities"])
		{
			std::string name = i["ability"]["name"];
			int slot = i["slot"];

			data.push_back({ "ability", name });
			data.push_back({ "slot", std::to_string(slot) });
		}

		for (const auto& i : jsonResponse["moves"])
		{
			std::string moveName = i["move"]["name"];

			data.push_back({ "move", moveName });
		}
	}

	//pokemon's moves
	else if (jsonResponse.contains("accuracy"))
	{
		int effectChance;
		int power;
		int accuracy;
		int priority;



		jsonResponse["effect_chance"].is_null() ? effectChance = 0 : effectChance = jsonResponse["effect_chance"];
		jsonResponse["power"].is_null() ? power = 0 : power = jsonResponse["power"];
		jsonResponse["accuracy"].is_null() ? accuracy = 0 : accuracy = jsonResponse["accuracy"];
		jsonResponse["priority"].is_null() ? priority = 0 : priority = jsonResponse["priority"];

		data.push_back({ "effect chance", std::to_string(effectChance) });
		data.push_back({ "power", std::to_string(power) });
		data.push_back({ "accuracy", std::to_string(accuracy) });
		data.push_back({ "priority", std::to_string(priority) });

		for (auto const& i : jsonResponse["effect_entries"])
		{
			if (i["language"]["name"] == "en")
			{
				std::string effect = i["effect"];
				std::string shortEffect = i["short_effect"];

				data.push_back({ "effect", effect });
				data.push_back({ "short effect", shortEffect });
			}
		}

	}

	return data;

}


void printPokemonInfo(const std::vector<std::pair<std::string, std::string>>& data)
{
	for (const auto& pair : data)
	{
		std::cout << pair.first << ':' << pair.second << std::endl;
	}
}


//menus


void inputMenu()
{
	std::string endpointArgument;

	system("CLS");
	std::cout << "Input:";
	std::cin >> endpointArgument;

	apiUrlEndpoint = "pokemon/" + endpointArgument + '/';
	apiUrl = apiUrlMainBody + apiUrlEndpoint;
}


void mainMenu()
{
	int input;
	std::string responseData, endpointArgument;



	std::vector<std::pair<std::string, std::string>> data;


	std::cout << "--POKESEARCH---\n";
	std::cout << "SEARCH ABOUT POKEMON INFO:\n";
	std::cout << "1. NAME\n";
	std::cout << "2. ABILITY\n";
	std::cout << "3. MOVE\n";

	std::cout << "Input:";
	std::cin >> input;

	switch (input)
	{
		case 1:			
			system("CLS");
			std::cout << "Input:";
			std::cin >> endpointArgument;

			apiUrlEndpoint = "pokemon/" + endpointArgument + '/';
			apiUrl = apiUrlMainBody + apiUrlEndpoint;

			responseData = returnRequestResult(apiUrl);
			data = extractResponseData(responseData);

			printPokemonInfo(data);

			std::cout << "Exit to main Menu? Yes : 1 ; No : any other number\n";
			std::cin >> input;

			system("CLS");
			if (input == 1) mainMenu();

			break;

		case 2:
			system("CLS");
			std::cout << "Input:";
			std::cin >> endpointArgument;

			apiUrlEndpoint = "ability/" + endpointArgument + '/';
			apiUrl = apiUrlMainBody + apiUrlEndpoint;

			responseData = returnRequestResult(apiUrl);
			data = extractResponseData(responseData);

			printPokemonInfo(data);

			std::cout << "Exit to main Menu? Yes : 1 ; No : any other number\n";
			std::cin >> input;

			system("CLS");
			if (input == 1) mainMenu();

			break;

		case 3:
			system("CLS");
			std::cout << "Input:";
			std::cin >> endpointArgument;

			apiUrlEndpoint = "move/" + endpointArgument + '/';
			apiUrl = apiUrlMainBody + apiUrlEndpoint;


			responseData = returnRequestResult(apiUrl);
			data = extractResponseData(responseData);

			printPokemonInfo(data);

			std::cout << "Exit to main Menu? Yes : 1 ; No : any other number\n";
			std::cin >> input;

			system("CLS");
			if (input == 1) mainMenu();
			
			break;


		default:
			std::cout << "Wrong! Try again\n";
			break;
	}
}
