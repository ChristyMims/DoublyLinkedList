////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  This program takes Homework assignment 2 (linkedLists) and modifies the code to create a doubly 
//  linked list. It will use the function that created the linked list in order of generation. The even 
//  numbers in the data will be deleted then the list will be printed in reverse order.  All the data will be
//  outputted to a file.  
//
//  Christy Mims
//
//  07/09/2019
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include<iostream>
#include<ctime>
#include<fstream>

using namespace std;

struct  node
{
	int number;
	node *next;
	node *previous;
};

// function headers
int promptUser(int);
int writeBinaryFile(int);
int generatedOrder(int);
void deleteEvenNums(node *);
void printLinkedList(node *);
void printReverseOrder(node *);




int main(int argc, char *argv[])
{
	// see if the list size was passed in from the command line
	if (argc == 2)
	{
		// cast the list size from ascii to int
		int listSize = atoi(argv[1]);

		// checks to see if valid list size was entered
		if ((listSize < 20) && (listSize > 100))
		{
			// calls the write binary function
			writeBinaryFile(listSize);

			// calls function to put generated order in linked list
			generatedOrder(listSize);
		}

		// if nonvalid number was entered
		else
		{
			// calls the promptUser function to get valid list size
			int listSize = 0;
			listSize = promptUser(listSize);

			// calls the write binary function
			writeBinaryFile(listSize);

			// calls function to put generated order in linked list
			generatedOrder(listSize);
		}
	}

	// if list size was not entered in the command line
	else
	{
		// calls the promptUser function to get list size
		int listSize = 0;
		listSize = promptUser(listSize);

		// calls the write binary function
		writeBinaryFile(listSize);

		// calls function to put generated order in linked list
		generatedOrder(listSize);

	}




	system("pause");
	return(0);

}

int promptUser(int listSize)
{

	// user prompt
	cout << "Enter a number between 20 and 100." << endl;

	// loop to make sure user enters number within range
	while (listSize < 20 || listSize > 100)
	{
		// take in list size from the user
		cin >> listSize;

		// formatting
		cout << endl;

		if (cin.fail() || listSize < 20 || listSize > 100)
		{
			// clears cin and the buffer
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			// notify the user to enter a number in specified range
			cout << endl << "Please enter a number between 20 and 100." << endl;
		}

	}

	// returns the amount of data the user wants to generate
	return(listSize);
}

int writeBinaryFile(int listSize)
{
	// create and name out stream writer 
	ofstream writeBinaryFile;

	// create the file and write it as binary
	writeBinaryFile.open("numbers.bin", ios::out | ios::binary);

	// check to see if file was opened
	if (!writeBinaryFile)
	{
		cout << "Binary file was not opened or created for writing." << endl << endl;
		return(2);
	}

	// 'seeds' the number random function with time from the computer
	srand(time(NULL));

	// tells user how much data will be generatedz
	cout << "List size to be generated " << listSize << endl;
	cout << "Start writing data to file." << endl << endl;

	// variable to hold the numbers that are randomly generated
	int fileNumbers = 0;

	// for loop to generate the amount of data the user specified
	for (int i = 0; i < listSize; i++)
	{
		// generate numbers between 0 and 99
		fileNumbers = (rand() * rand()) % 100;

		// outputs the numbers to the console for user to see
		cout << fileNumbers << endl;

		// writes numbers to the binary file
		writeBinaryFile.write(reinterpret_cast<const char *>(&fileNumbers), sizeof(int));
	}

	// formatting
	cout << endl << endl;

	// closes the binary file
	writeBinaryFile.close();

}

int generatedOrder(int listSize)
{
	// creates pointer to head and keeps track of beginning of list
	struct node *Head;

	// creates a new nod and sets it equal to head
	Head = new(node);

	// checks to see if new node got created
	if (Head == NULL)
	{
		cout << endl << "Fatal error, could not create head node." << endl;
		//system("PAUSE");
		return(1);
	}

	// if head was created it sets head pointer to null since it is last in the list
	Head->next = NULL;
	Head->previous = NULL;

	// creates the stream reader
	ifstream generatedOrder;

	// opens the file
	generatedOrder.open("numbers.bin", ios::in | ios::binary);

	// lets the user know if the file did not open
	if (!generatedOrder)
	{
		cout << "Could not open binary file for reading" << endl << endl;
		return(2);
	}

	// lets the user know reading from file has begun
	cout << "Begin reading from file" << endl;
	//
	cout << "Generated Order: " << endl << endl;

	// loop until reaches end of file
	//for (int i = 0; i < listSize; i++)
	while(generatedOrder)
	{
		// set equal to head so can traverse list  without losing head
		struct node *Current = Head;

		// creates pointer to new node
		struct node *newNode;

		// read in from file
		int num = 0;
		generatedOrder.read((char *)&num, sizeof(int));

		if (generatedOrder.eof()) break;

		// while loop to look for the end of the list
		while (Current->next != NULL)
		{
			// increments current pointer
			Current = Current->next;
		}

		// creates new node and check if memory was allocated
		// outputs if not enough memory
		if ((newNode = new(node)) == NULL)
		{
			cout << "Not enough memory!!" << endl;
			return(1);
		}
		
		// puts data into new node
		newNode->number = num;

		// debugging purposes
		// output to know if data was put into new node
		//cout << "newnode-> " << newNode << "\t" << "Data -> " << newNode->number << endl;

		// inserts the node into the linked list at the end
		newNode->next = NULL;
		newNode->previous = Current;
		Current->next = newNode;
		
		// for debugging purposes
		//cout << "Current -> " << Current << "\t" << "Current->next" << Current->next << endl;
	}

	// prints and formats the linked list
	printLinkedList(Head);

	// notifies user
	cout << "List without even numbers:" << endl;

	// calls the function to delete even numbers in list and print to the console
	deleteEvenNums(Head);
	printLinkedList(Head);

	// calls the function to print list in reverse order
	printReverseOrder(Head);

	// close the file
	generatedOrder.close();
}


void printLinkedList(node *Head)
{
	// create a temp pointer and set it equal to head
	struct node *temp;
	temp = Head->next;


	// used for formatting output to the console
	int counter = 1;

	while (temp != NULL)
	{
		// print the node then a tab to separate the numbers
		cout << temp->number << "\t";

		// formatting: after every 5 nodes are printed the a newline is inserted
		// then the counter is reset
		if (counter % 5 == 0)
		{
			cout << endl;
			counter = 0;
		}

		// advances pointer
		temp = temp->next;

		// increments the counter
		counter = counter + 1;

	}

	// formatting for console
	cout << endl;
}

void deleteEvenNums(node *Head)
{
	// create temp node to traverse list
	struct node *temp;
	struct node *current = Head->next;

	// debugging purposes
	//cout << "head -> " << Head << "\t temp -> " << temp << endl;


	// Loops through the list and deletes any even number
	while (current->next != NULL)
	{
		//temp = Head->next;

		// if the number in the node is even it will be deleted
		if (current->number % 2 == 0)
		{
			temp = current;
			current = current->previous;

			temp->previous->next = temp->next;
			temp->next->previous = temp->previous;

			// for debugging purposes
			//cout << "temp->next " << temp->next << "\t" << "temp->previous->next " << "temp->previous->next" << endl;
			//cout << "temp->next->previous " << temp->next->previous << "\t" <<"temp->previous " << temp->previous << endl;

			delete temp;

		}

		// increment pointer
		current = current->next;
	}

	// executes if the last number in the list is even as well
	if (current->number % 2 == 0)
	{
		current->previous->next = NULL;
		delete current;
	}
}

void printReverseOrder(node *Head)
{
	// formatting
	cout << endl;

	// create pointer to traverse the list
	struct node *Current;
	Current = Head->next;
	
	// let the user know the list will now be printed in reverse order
	cout << "Reverse Order: " << endl;

	// looks for the end of the list
	while (Current->next != NULL)
	{
		Current = Current->next;
	}

	// prints out the list in reverse order
	while (Current->previous != NULL)
	{
		cout << Current->number << "\t";
		Current = Current->previous;
	}
	
	// formatting
	cout << endl << endl;

}
