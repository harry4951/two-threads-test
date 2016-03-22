/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <queue>

using namespace std;
//static mutex barrier;
//
//static const int num_threads = 2;
//vector_atomic<>numberlist;  //vector to store the numerical values
////This function will be called from a thread
//
//     void call_from_thread(int tid) {
//      cout << "Launched by thread " << tid << endl;
//	  
//}
//	 void read_doc(ifstream &myfile)
//     {
//		 float number = 0;
//		 while (myfile >> number){    //
//			 numberlist.push_back(number);
//		 }
//		 lock_guard<mutex> block_threads_until_finish_this_job(barrier);
//     }
//
//	 void display()
//     {
//		 for (int i = 0; i < numberlist.size(); i++){
//			 cout << numberlist[i] << endl;
//		 }
//		 lock_guard<mutex> block_threads_until_finish_this_job(barrier);
//     }
////==============================================================================
//int main (int argc, char* argv[])
//{
//	//float f1 = -187.33667, f2 = 0.0;
//	//float *float_arry;
//
//	thread t[num_threads];
//	/*
//	         //Launch a group of threads
//	         for (int i = 0; i < num_threads; ++i) {
//				 t[i] = std::thread(call_from_thread, i);
//	
//	}
//	
//	         cout << "Launched from the main\n";
//	
//	         //Join the threads with the main thread
//	         for (int i = 0; i < num_threads; ++i) {
//	             t[i].join();
//	
//	}
//	*/
///*
//	//Launch a thread
//	thread t1(call_from_thread);
//	//Join the thread with the main thread
//	t1.join();
//*/
//	ifstream infile("myfile.txt");
//
//	if (!infile.is_open())  // check file is open, quit if not
//	{
//		cerr << "failed to open file\n";
//		return 1;
//	}
//	
//	t[0] = thread(read_doc, infile);
//	t[1] = thread(display);
//	t[0].join();
//	t[1].join();
//	//vector<float>numberlist;  //vector to store the numerical values
///*	float number = 0;
//	while (myfile >> number){    //
//		numberlist.push_back(number);
//	}
//*/
///*		for (int i = 0; i < numberlist.size(); i++){
//			cout << numberlist[i] << endl;
//		}
//*/
//	ofstream outfile("test.txt");
//	for (int i = 0; i < numberlist.size(); i++){
//		outfile << numberlist[i] << endl;
//		cout << numberlist[i] << endl;
//	}
//	
//	outfile.close();
//	
//
//	while (1);
//	return 0;
//}




#include <condition_variable>

template <typename T>
class Queue
{
public:

	T pop()
	{
		unique_lock<mutex> mlock(mutex_);
		while (queue_.empty())
		{
			cond_.wait(mlock);
		}
		auto item = queue_.front();
		queue_.pop();
		return item;
	}

	void pop(T& item)
	{
		unique_lock<mutex> mlock(mutex_);
		while (queue_.empty())
		{
			cond_.wait(mlock);
		}
		item = queue_.front();
		queue_.pop();
	}

	void push(const T& item)
	{
		unique_lock<mutex> mlock(mutex_);
		queue_.push(item);
		mlock.unlock();
		cond_.notify_one();
	}

private:
	queue<T> queue_;
	mutex mutex_;
	condition_variable cond_;
};

class Producer
{
public:
	Producer(Queue<float>& buffer)
		: queue_(buffer)
	{}

	void run() {
		while (true) {
			float num = rand() % 100;
			queue_.push(num);
			
			cout << "Produced: " << num << endl;
			this_thread::sleep_for(chrono::seconds(5));
			
		}
	}
private:
	Queue<float>&  queue_;
};

class Consumer
{
public:
	
	Consumer(Queue<float>& buffer)
		: queue_(buffer)
	{}

	void run() {
		while (true) {
			float num = queue_.pop();
			cout << "Consumed: " << num << endl;
			this_thread::sleep_for(chrono::seconds(5));
		}
	}
private:
	Queue<float>&  queue_;
};

int main() {
	Queue<float> b;
	Producer p(b);
	Consumer c(b);

	std::thread producer_thread(&Producer::run, &p);
	std::thread consumer_thread(&Consumer::run, &c);

	producer_thread.join();
	consumer_thread.join();
	getchar();
	return 0;
}