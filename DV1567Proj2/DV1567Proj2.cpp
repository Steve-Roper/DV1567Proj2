// DV1567proj2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include "time.h"
#include "stdlib.h"
#include <vector>
#include <chrono>

#define CURRENT_TIME duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()
using namespace std;
using namespace chrono;

void createDataset(int datasetSize, string filename, int bufferSize);
vector<float> loadDataset(int datasetSize, string filename, int bufferSize);
float average(vector<float> dataset);
float minValue(vector<float> dataset);
float maxValue(vector<float> dataset);
vector<float> insertionsort(vector<float> dataset);
void writeDataset(string filename, vector<float> sortedDataset, int bufferSize);
int cmpfunc(const void * a, const void * b);

int main()
{
	srand(time(NULL));
	int datasetSize = 4096;
	int bufferSize = 1;
	ofstream f;
	f.open("results_qsort.txt");
	if (f.is_open())
	{
		for (int i = 0; i < 3; ++i)
		{
			string filename = "dataset_" + to_string(datasetSize) + ".txt";
			//createDataset(datasetSize, filename, bufferSize);
			long long start = CURRENT_TIME;
			vector<float> dataset = loadDataset(datasetSize, filename, bufferSize);
			f << "-----loadDataset-----\n\tdataset size: " << datasetSize << "\n\tbuffer size: " << bufferSize << "\n\tmsec taken: " << CURRENT_TIME - start << endl;
			cout << "Milliseconds taken by loadDataset to read " << datasetSize << " floating point numbers from a file: " << CURRENT_TIME - start << endl;
			float avg = average(dataset);
			float min = minValue(dataset);
			float max = maxValue(dataset);
			start = CURRENT_TIME;
			//dataset = insertionsort(dataset);
			qsort(dataset.data(), dataset.size(), sizeof(float), cmpfunc);
			f << "-----sortDataset-----\n\tdataset size: " << datasetSize << "\n\tbuffer size: " << bufferSize << "\n\tmsec taken: " << CURRENT_TIME - start << endl;
			cout << "Milliseconds taken by sortDataset to sort " << datasetSize << " floating point numbers: " << CURRENT_TIME - start << endl;
			filename = "sortedDataset" + to_string(datasetSize) + "_" + to_string(bufferSize) + "qsort.txt";
			start = CURRENT_TIME;
			writeDataset(filename, dataset, bufferSize);
			f << "-----writeDataset-----\n\tdataset size: " << datasetSize << "\n\tbuffer size: " << bufferSize << "\n\tmsec taken: " << CURRENT_TIME - start << endl;
			cout << "Milliseconds taken by writeDataset to write " << datasetSize << " floating point numbers to a file: " << CURRENT_TIME - start << endl;
			datasetSize *= 2;
		}
		datasetSize = 4096;
		bufferSize *= 2;
		for (int i = 0; i < 4; ++i)
		{
			string filename = "dataset_" + to_string(datasetSize) + ".txt";
			//createDataset(datasetSize, filename, bufferSize);
			long long start = CURRENT_TIME;
			vector<float> dataset = loadDataset(datasetSize, filename, bufferSize);
			f << "-----loadDataset-----\n\tdataset size: " << datasetSize << "\n\tbuffer size: " << bufferSize << "\n\tmsec taken: " << CURRENT_TIME - start << endl;
			cout << "Milliseconds taken by loadDataset to read " << datasetSize << " floating point numbers from a file: " << CURRENT_TIME - start << endl;
			float avg = average(dataset);
			float min = minValue(dataset);
			float max = maxValue(dataset);
			start = CURRENT_TIME;
			//dataset = insertionsort(dataset);
			qsort(dataset.data(), dataset.size(), sizeof(float), cmpfunc);
			f << "-----sortDataset-----\n\tdataset size: " << datasetSize << "\n\tbuffer size: " << bufferSize << "\n\tmsec taken" << CURRENT_TIME - start << endl;
			cout << "Milliseconds taken by sortDataset to sort " << datasetSize << " floating point numbers: " << CURRENT_TIME - start << endl;
			filename = "sortedDataset" + to_string(datasetSize) + "_" + to_string(bufferSize) + "qsort.txt";
			start = CURRENT_TIME;
			writeDataset(filename, dataset, bufferSize);
			f << "-----writeDataset-----\n\tdataset size: " << datasetSize << "\n\tbuffer size: " << bufferSize << "\n\tmsec taken: " << CURRENT_TIME - start << endl;
			cout << "Milliseconds taken by writeDataset to write " << datasetSize << " floating point numbers to a file: " << CURRENT_TIME - start << endl;
			bufferSize *= 2;
		}
		f.close();
	}
	system("PAUSE");
}

/*
* Create dataset with random floating point numbers ranging from 0-100
*/
void createDataset(int datasetSize, string filename, int bufferSize)
{
	vector<float> dataset;
	for (int i = 0; i < datasetSize; ++i)
		dataset.push_back((float)rand() / (float)(RAND_MAX / 100.f));

	FILE* f;
	f = fopen(filename.c_str(), "wb");
	if (f)
	{
		for (int i = 0; i < datasetSize; ++i)
			fwrite(&dataset[i], sizeof(float), bufferSize, f);
		fclose(f);
	}
	else
		cout << "Could not open file \'" << filename << "\'" << endl;
}

/*
* Load dataset from file
*/
vector<float> loadDataset(int datasetSize, string filename, int bufferSize)
{
	vector<float> dataset;
	FILE* f;
	f = fopen(filename.c_str(), "rb");
	if (f)
	{
		float temp[16] = { 0.f };
		for (int i = 0; i < datasetSize / bufferSize; ++i)
		{
			fread(temp, sizeof(float), bufferSize, f);
			for(int j = 0; j < bufferSize; ++j)
				dataset.push_back(temp[j]);
		}
		fclose(f);
	}
	else
		cout << "Could not open file \'" << filename << "\'" << endl;

	return dataset;
}

/*
* Returns mean value of the dataset
*/
float average(vector<float> dataset)
{
	float average = 0;

	for (int i = 0; i < dataset.size(); ++i)
		average += dataset[i];

	return average /= (float)dataset.size();
}

/*
* Returns min value from dataset
*/
float minValue(vector<float> dataset)
{
	float min = dataset[0];

	for (int i = 1; i < dataset.size(); ++i)
	{
		if (dataset[i] < min)
		{
			min = dataset[i];
		}
	}

	return min;
}

/*
* Returns max value from dataset
*/
float maxValue(vector<float> dataset)
{
	float max = dataset[0];

	for (int i = 1; i < dataset.size(); ++i)
	{
		if (dataset[i] > max)
		{
			max = dataset[i];
		}
	}

	return max;
}
int cmpfunc(const void * a, const void * b) {
	return (*(int*)a - *(int*)b);
}

/*
* Returns a sorted version of dataset
*/
vector<float> insertionsort(vector<float> dataset)
{
	vector<float> sorted;

	sorted.push_back(dataset[0]);
	//loop through our dataset
	for (int i = 1; i < dataset.size(); ++i)
	{
		//loop through our sorted list
		for (int j = 0; j < sorted.size(); ++j)
		{
			//compare values in sorted list to dataset
			if (dataset[i] <= sorted[j])
			{
				//shift values in sorted list to make room for new item
				sorted.push_back(sorted[sorted.size() - 1]);
				for (int k = sorted.size() - 1; k > j; --k)
					sorted[k] = sorted[k - 1];
				sorted[j] = dataset[i];
				break;
			}
			//if the number is larger than all numbers in our sorted list
			else if (j == sorted.size() - 1)
			{
				sorted.push_back(dataset[i]);
				break;
			}
		}
	}
	return sorted;
}

/*
* Writes the sorted dataset into the file.
*/
void writeDataset(string filename, vector<float> sortedDataset, int bufferSize)
{
	FILE* f;
	f = fopen(filename.c_str(), "wb");
	if (f)
	{
		for (int i = 0; i < sortedDataset.size() / bufferSize; ++i)
			fwrite(&sortedDataset[i], sizeof(float), bufferSize, f);
		fclose(f);
	}
	else
		cout << "Could not open file \'" << filename << "\'" << endl;
}