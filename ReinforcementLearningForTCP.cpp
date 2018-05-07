
//

//  ReinforcementLearningForTCP.cpp

//

//  Created by Sanjay on 06/05/18.

//  Copyright © 2018 Sanjay Reddy. All rights reserved.

//


#include <iostream>
#include <list>
#include <map>
#define INC_CWND 0
#define DEC_CWND 1

using namespace std;

map<list<uint32_t>, list<int>> qmap;

void updateQValue(list<uint32_t> key, int action, int reward)
{
	map<list<uint32_t>, list<int>>::const_iterator poss = qmap.find(key);
	if (poss != qmap.end())
	{
		list<int> val = poss->second;
		int array[2];
		int i = 0;
		for (int n : val)
		{
			array[i] = n;
			i++;
			if (i == 2)
				break;
		}
		array[action] = reward;
		list<int> action_list = {array[0],array[1]};
		qmap.at(key) = action_list;
	}
}

void printMap()
{
	for (auto elem : qmap)
	{
		list<uint32_t> key = elem.first; 
		list<int> val = elem.second;
		for (uint32_t n : key)
		{
			cout << n << ",";
		}
		cout << "\b";
		cout << " -> ";
		for (int n : val)
		{
			cout << n << ",";
		}
		cout << "\n";
	}
}

// The reward function is used to get proper reward for the parameters input. 

int getReward(list<uint32_t> key,int action)
{
	int reward = 100;
	if (action == DEC_CWND)
	{
		reward = 50;
	}
	return reward;
}

//This function uses a variation of Q learning to predict whether we should increase or decrease the congestion window size.
void change_cwnd(uint32_t a, uint32_t b)
{
	int action = 0;
	list<uint32_t> key = {a,b};
	map<list<uint32_t>, list<int>>::const_iterator poss = qmap.find(key);
	bool isNoUpdate = false;
	if (poss == qmap.end())
	{
		// there was no such state registered. Hence we enter anew state.
		list<int> val = {-1,-1};
		qmap.emplace(key,val);
		action = rand() % 1;
	}
	else
	{
		int array[2];
		int i = 0;
		list<int> val = poss->second;
		for (int n : val)
		{
			array[i] = n;
			i++;
			if (i == 2)
				break;
		}
		if (array[0] >= 0 && array[1] >= 0)
		{
			isNoUpdate = true;
			if (array[0]>array[1])
				action = INC_CWND;
			else
				action = DEC_CWND;
		}
		else if (array[0] < 0 && array[1] < 0)
		{
			action = rand() % 1;
		}
		else if (array[0] < 0)
		{
			action = INC_CWND;
		}
		else if (array[1] < 0)
		{
			action = DEC_CWND;
		}
	}
	if (isNoUpdate)
	{
		cout << "Selected action from optimal Q value function is: " << action << endl;
		//just re transmit using the corresponding action
	}
	else
	{
		int reward = getReward(key, action);
		if (action == INC_CWND)
		{
			updateQValue(key, INC_CWND, reward);
		}
		else
		{
			updateQValue(key, DEC_CWND, reward);
		}
		//re transmit using the corresponding action
	}
	printMap();
}



int main(int argc, const char * argv[]) {

	// initialise the reward function, reward function can be based on the output. if there is wireless loss or not. If there is wireless loss and we predicted a congestion less.
	change_cwnd(1,1);
	change_cwnd(1, 1);
	change_cwnd(3,1);
	change_cwnd(4,1);
	change_cwnd(1, 1);
	while (1);
	return 0;

}


