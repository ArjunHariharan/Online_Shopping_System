/*
This is a small online shopping application demonstrating the concepts of hashing.
The following features are included in the program.
-Username verification
-Password verification
-Search Order
-Maintain histrory of orders placed
-Features allowing removal of orders from the cart.
-Automatic Billing

This application can handle any number of users.
A check on the input data has been put to ensure.
This application was developed in March of 2012 as a part of mini project for 4th semester, Bachelor of Engineering, Computer Science.

Team Members- Arjun Hariharan
Kuldeep Ghate
*/

#include<iostream>
#include<conio.h>
#include<fstream>
#include<string>
#include<process.h>
#include<stdio.h>
#include<list>
#include<iomanip>
#include<ctype.h>
using namespace std;

class data
{
protected:
	char address[51], name[31], gender, profession[16], dob[11], email[31], nationality[21];
	friend class login;
};

class login : private data
{
	friend class order;  //products need to be enterted with the data
	char password[21];
	int long offset_next, offset_order;
protected:
	char username[11];
public:
	login()
	{
		offset_next = 0;
		offset_order = 0;
	}

	int verify_password();
	int getdata();

	friend void signin();
	friend void signup();
	friend long int search(char[], long int, int *);  //existing user
	friend int access(long int);
	friend void putdata(login);
	friend void show(login);
	friend void store_product(long int);
	friend void feed(long int);
};

class order
{
	char product_name[20];
	int order_no;
	float cost;
public:
	order()
	{
		order_no = 0;
		offset_same_user = 0;
	}
	long int offset_same_user;
	friend int laptops();
	friend int phones();
	friend void cart();
	friend void store_product(long int);
	friend void feed(long int);
	friend void feed(long int);
};

void signin();
void signup();
void segments();
int numeric(char[]);

list <order> head;
list <order> ::iterator temp;

void main()
{
	char c;
	long int j = 0;
	char username[11];
	int option = 0;
	login l;
	system("cls");

	/*
	//This code segment has to be run once to initialize files to store the index and other details.
	// Consider this segment of code like a setup for the program.
	ofstream edit("data.txt",ios::trunc |ios ::out);
	edit.close();

	ofstream set("index.txt",ios::out | ios::trunc); // run only once to  initialize the index file at the start
	for(int i=0;i<=11000;i++)
	{
	set.seekp(i,ios::cur);
	}
	set.close();
	ofstream product("order.txt");
	j=1;
	product<<j;
	for(int i=0;i<5;i++)
	{
	product.seekp(1,ios::cur);
	}
	c='$';
	product.put(c);
	product.close();
	exit(0);
	*/
	do
	{
		while ((cout << "\nEnter\n\n\n1.Login\n\n\n2.New user\n\n\n3.Exit\n\n") && (!(cin >> option) || option < 1 || option > 3))   //checks if the input is correct
		{
			std::cout << "\nInvalid Choice\n";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		switch (option)
		{
		case 1:
			signin();
			break;
		case 2:
			signup();
			system("cls");
			break;
		case 3:
			cout << "\nExiting...\n";
			break;
		default:
			cout << "Invalid Choice\n";
			break;
		}
	} while (1);
	getch();
}

int numeric(char str[])
{
	for (int i = 0; i<10; i++)
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
		{
			return(0);
		}
	}
	return(1);
}

void signup() //new user
{
	char password[20], c, u_name[11];
	long int pos_index, offset, prev;
	int key = 0, flag = 0; //flag to indicate if user exists
	int abort_signup = 0;

enter:
	login l;
	cout << "\nEnter your phone number as your username.\nEx-9482713723\n";
	flushall();
	fgets(u_name, 11, stdin);

	if (strlen(u_name) != 10)
	{
		cout << "\nMobile no incorrect. Enter again\n";
		goto enter;
	}
	if (u_name[0] == '0')
	{
		cout << "\nEnter mobile no without 0.\n";
		goto enter;
	}
	if (!numeric(u_name))
	{
		cout << "\nUsername should be numeric (mobile no).Enter again\n";
		goto enter;
	}

	key += (u_name[9] + (u_name[8] * 10) + (u_name[7] * 100));  //calculate key
	key -= 5328;  //char to int type conversion of mod 10 of no

	pos_index = key * 11;  //calulate offset of key

	ifstream index_in("index.txt", ios::in | ios::_Nocreate);
	index_in.seekg(pos_index, ios::beg);  //move get pointer to offset of key

	index_in.get(c);
	if (index_in.fail())  //no username.create account
	{
		ifstream data_in("data.txt", ios::in | ios::ate | ios::_Nocreate | ios::binary);  //opens the file to read the last postition so that the next user data can be added from there
		offset = data_in.tellg();  // this offset stores the position where the user data can be added. This adress is then stored in the index table
		data_in.close();

		strcpy(l.username, u_name);
		abort_signup = l.verify_password();
		if (abort_signup == 1)   // check if user cancelled signup
		{
			abort_signup = l.getdata();
			if (abort_signup == 1)
			{// User has entered all the details. Create account
				ofstream feed("index.txt", ios::_Nocreate | ios::binary);  //stream is lost as it enters the condition
				feed.seekp(pos_index, ios::beg);  //move put pointer to offset
				feed << offset;  //save the offset in index file
				feed.close();
				putdata(l);
				cout << "\nAccount Created\n";
			}
		}
	}

	else //username may exist. search 
	{
		ifstream take("index.txt", ios::binary | ios::_Nocreate);

		take.seekg(pos_index, ios::beg);

		take >> offset;	//		take.read((char *)&l,sizeof(l));
		take.close();

		offset = search(u_name, offset, &flag);

		if (flag == 0)  //username exists
		{
			system("cls");
			cout << "\nUsername exist.\nEnter 1 to create new account or 0 to exit\n";
			cin >> key;
			if (key == 1)  //repeat signup function
			{
				goto enter;
			}
			else
			{
				exit(0);
			}
		}
		else
		{
			login n;
			prev = offset;  //save offset of previous node in prev variable
			fstream fin("data.txt", ios::ate | ios::_Nocreate | ios::binary | ios::in | ios::out);  //data file to be edited

			offset = fin.tellp();  //get offset of new user

			fin.seekp(prev, ios::beg);
			fin.read((char *)&l, sizeof(l)); //read previous entry to change offset 

			l.offset_next = offset;  //change offset from 0 to next entry offset
			fin.seekp(prev, ios::beg);
			flag = fin.tellp();
			fin.write((char *)&l, sizeof(l)); //write back modified user detail

			fin.close();

			strcpy(n.username, u_name);
			n.verify_password();
			n.getdata();
			putdata(n);
		}
	}
	index_in.close();
}

void signin()  //signin to shop
{
	long int pos_index, pos_data;
	int key = 0, choice, flag = 0, size;
	char user[11], c, pass[20];
	login user_detail;
	order o;

	cout << "\nEnter the username\n";
	flushall();
	gets(user);

	key += (user[9] + (user[8] * 10) + (user[7] * 100));  //calculate key
	key -= 5328;

	ifstream index_in("index.txt", ios::in | ios::_Nocreate);
	pos_index = key * 11;  //offset of key in index file

	index_in.seekg(pos_index, ios::beg);  //bring get pointer to offset
	index_in.get(c);

	if (index_in.fail())  //user doesn't exist
	{
	sign:
		cout << "\nUsername doesn't exist\nEnter 1 to signup\nEnter 0 to exit\n";
		cin >> choice;
		if (choice == 1)
		{
			signup();
		}
		else
		{
			exit(0);
		}
	}

	else  //username may exist.check
	{
		ifstream take("index.txt", ios::in | ios::_Nocreate);  //stream doesn't work as it enters the condition.

		take.seekg(pos_index, ios::beg);  //take get pointer to offset of key
		take >> pos_index;  //accept the offset of the user
		take.close();

		pos_data = search(user, pos_index, &flag);  //returns the offset

		if (flag == 1)  //create user
		{
			goto sign;
		}
		else  //existing user
		{
			flag = access(pos_data);
			if (flag == 1)  //username and password valid
			{
				//show(user_detail);
				segments(); //display products
				store_product(pos_data); //store the products in the file	
			}
		}
	}
	index_in.close();
}

int login::verify_password()  //verify the password before signup
{
	char pass[21];
	do
	{
		cout << "\nEnter password (min 6 char and max 20 char) or Enter exit to quit\n";
		flushall();
		fgets(password, 21, stdin);

		if (stricmp(password, "exit\n") == 0)
		{
			cout << "Account Creation Aborted";
			return(0);
		}

		if (strlen(password) < 7)
		{
			cout << "\nPassword out of range. Enter again\n";
		}
		else
		{
			if (strlen(password) >20)
			{
				cout << "\nPassword long. Max 20 char allowed.\n";
			}
			else
			{
				cout << "\nRe enter password\n";
				flushall();
				fgets(pass, 21, stdin);
				if (strcmp(password, pass) != 0)  //if password doesn't match
				{
					cout << "\nPassword incorrect.\nEnter again.\n";
				}
				else
				{
					break;
				}
			}
		}
	} while (1);

	system("cls");
	return(1);
}

long int search(char username[], long int offset, int *flag) //returns the first offset address of the user ie from username
{
	int status = 0;
	long int prev = 0;
	//login l;

	do
	{
		login l;
		ifstream search("data.txt", ios::in | ios::binary | ios::_Nocreate);  //open data file
		search.seekg(offset, ios::beg);

		search.read((char*)&l, sizeof(l));
		flushall();
		if (strcmp(l.username, username) == 0)  //if user found then return offset
		{
			*flag = 0;  //indicates that user exists
			return(offset);  //points to username[0] of the next object
		}
		else
		{
			if (l.offset_next == 0)
			{
				status = 0;
			}
			else
			{
				prev = offset;
				offset = l.offset_next;
				status = 1;
			}
		}
		search.close();
	} while (status == 1);  //condition.

	if (offset == 0)
	{
		*flag = 1;  //indicates that username doesnt exist
		return(prev);  //returns previous link of the same key.points to the first location ie username[0].
	}
}

int access(long int offset)  //checks the passowrd of existing user
{
	login user_detail;
	int	flag = 0;
	char p[21];

	ifstream data_file("data.txt", ios::in | ios::_Nocreate | ios::binary);
	data_file.seekg(offset, ios::beg);
	data_file.read((char *)&user_detail, sizeof(user_detail)); //read the contents of the object

pass:
	if (flag == 3)   //accept password for 3 times else exit
	{
		cout << "\nAccount blocked.\n";
		return(0);
	}
	else
	{
		cout << "\nEnter password\n";
		flushall();
		fgets(p, 21, stdin);  //get password from user

		if (strcmp(p, user_detail.password) == 0)  //if password matches thn provide access
		{
			cout << "\nAccess granted\n";
			getch();
			return(1);
		}
		else
		{
			cout << "\nIncorrect password.\n" << 2 - flag << " turns left.\n";
			flag++;
			goto pass;
		}

	}
}

int login::getdata()  //function to accept user data 
{
	char status[5];
	cout << "\nEnter your personal details.\n";

	cout << "\Name(30 char) or Enter exit to quit ";
	flushall();
	fgets(name, 31, stdin);
	if (stricmp(name, "exit") == 0)
	{
		cout << "Account Creation Aborted";
		return(0);
	}

rep_gender:
	cout << "\nGender (M/F) or Enter exit to quit- ";
	flushall();
	fgets(status, 5, stdin);
	if (stricmp(status, "exit") == 0)
	{
		cout << "Account Creation Aborted";
		return(0);
	}
	if ((status[0] == 'm') || (status[0] == 'M') || (status[0] == 'f') || (status[0] == 'F'))
	{
		gender = toupper(status[0]);
	}
	else
	{
		cout << "\n Invalid entry. Enter again\n";
		goto rep_gender;
	}

rep_dob:
	cout << "\nDate of birth(dd-mm-yyyy) or Enter exit to quit- ";
	flushall();
	fgets(dob, 11, stdin);
	if (stricmp(dob, "exit") == 0)
	{
		cout << "Account Creation Aborted";
		return(0);
	}
	if (strlen(dob) != 10)
	{
		cout << "\n Invalid entry. Enter again\n";
		goto rep_dob;
	}

rep_add:
	cout << "\nAddress(50 char) or Enter exit to quit - ";
	flushall();
	fgets(address, 51, stdin);
	if (stricmp(status, "exit") == 0)
	{
		cout << "Account Creation Aborted";
		return(0);
	}
	if (strlen(address)<15)
	{
		cout << "\nInvalid Address.Enter Again\n";
		goto rep_add;
	}

	cout << "\nNationality or Enter exit to quit- ";
	flushall();
	fgets(nationality, 21, stdin);
	if (stricmp(status, "exit") == 0)
	{
		cout << "Account Creation Aborted";
		return(0);
	}

	cout << "\nProfession (15 char) or Enter exit to quit - ";
	flushall();
	fgets(profession, 16, stdin);
	if (stricmp(status, "exit") == 0)
	{
		cout << "Account Creation Aborted";
		return(0);
	}

rep_email:

	cout << "\nEmail - or Enter exit to quit ";
	flushall();
	fgets(email, 31, stdin);
	if (stricmp(status, "exit") == 0)
	{
		cout << "Account Creation Aborted";
		return(0);
	}
	if (strlen(email)<6)  //@some_website.com must be atleast 6 characters long
	{
		cout << "\nInvalid Email. Enter again\n";
		goto rep_email;
	}
	return(1);
}

void putdata(login l)  //to put the user data in the file
{
	ofstream data_out("data.txt", ios::out | ios::ate | ios::binary | ios::_Nocreate);
	data_out.write((char *)&l, sizeof(l));
	data_out.close();
}

int phones()  //Phone segment display
{
	order o;
	int choice;
	char c;
	system("cls");
	ifstream fin("phones.txt", ios::in | ios::_Nocreate);
	while (!fin.eof())
	{
		fin.get(c);
		cout << c;
	}
	cout << "\nEnter 1 to add to the cart\n\nEnter 0 to go back to Segmnts\n\n";
	cin >> choice;
	if (choice == 1)
	{
		cout << "\nEnter The product no\n";  //think of a cout for this statement
		cin >> choice;
		switch (choice)
		{
		case 1:
			strcpy(o.product_name, "REZOUND");
			o.cost = 10354;
			break;
		case 2:
			strcpy(o.product_name, "VOLTA");
			o.cost = 15550;
			break;
		case 3:
			strcpy(o.product_name, "DEZIRE");
			o.cost = 23990;
			break;
		default:
			cout << "\nInvalid product no\n";
			exit(0);
			break;
		}
		head.push_back(o);
		cout << "\nProduct added to the cart\n";
		getch();
	}
	return(choice);
}

int laptops()  // Laptop segment page.
{
	order o;
	int page, choice;
	char c;
page_1:
	system("cls");

	ifstream fin("laptops_1.txt", ios::in | ios::_Nocreate);
	while (!fin.eof())
	{
		fin.get(c);
		cout << c;
	}
	cout << "\n\n\nEnter 2 to go to next page\n\nEnter 3 to add to cart\n\n4.Go back to segment option\n\n";
	cin >> page;
	if (page == 2)
	{
		system("cls");
		ifstream fin1("laptops_2.txt", ios::in | ios::_Nocreate);
		while (!fin1.eof())
		{
			fin1.get(c);
			cout << c;
		}
		cout << "\n\n\nEnter 1 to go to previous page\n\nEnter 3 to add to cart\n\nEnter 4 to go back to segment option\n\n";
		cin >> page;
		if (page == 1)
		{
			goto page_1;
		}
		if (page == 3)
		{
			goto c;
		}
	}
	if (page == 3)
	{
	c:
		cout << "\nEnter The product no\n";  //think of a cout for this statement
		cin >> choice;
		switch (choice)
		{
		case 1:
			strcpy(o.product_name, "INSPIRE 14Z");
			o.cost = 42545;
			break;
		case 2:
			strcpy(o.product_name, "INSPIRE 15Z");
			o.cost = 51250;
			break;
		case 3:
			strcpy(o.product_name, "INSPIRE 17Z");
			o.cost = 65500;
			break;
		case 4:
			strcpy(o.product_name, "INSPIRE 14R");
			o.cost = 75500;
			break;
		default:
			cout << "\nInvalid product no\n";
			exit(0);
			break;
		}
		head.push_back(o);
		cout << "\nProduct added to cart\n";
		getch();
		return(1);
	}
	if (page == 4)
	{
		return(0);
	}
}

void show(login l)  //display the user data
{
	system("cls");
	cout << "\Name- ";
	puts(l.name);

	cout << "\nGender- " << l.gender;


	cout << "\nDate of birth- ";
	flushall();
	puts(l.dob);

	cout << "\nAddress- ";
	flushall();
	puts(l.address);

	cout << "\nNationality - ";
	flushall();
	puts(l.nationality);

	cout << "\nProfession- ";
	flushall();
	puts(l.profession);

	cout << "\nEmail - ";
	flushall();
	puts(l.email);
	getch();
	system("cls");
}

void segments()   // display the list of segments and show the products in each segment
{
	int option, flag;
	order o;
a:
	system("cls");
	cout << "\n\n\nSelect option\n\n\n1.Smart phone\n\n\n2.Laptops\n\n\n3.Exit\n\n\n";
	cin >> option;
	switch (option)
	{
	case 1:
		flag = phones();
		break;
	case 2:
		flag = laptops();
		break;
	case 3:
		exit(0);
		break;
	default:
		cout << "\nInvalid choice\n";
		break;
	}
	if (flag == 0)
	{
		goto a;
	}
	else
	{
		system("cls");
		cout << "\n\nEnter 1 to add more items to cart\n\nEnter 2 to buy\n\nEnter 3 to exit\n\n";
		cin >> option;
		if (option == 1)
		{
			goto a;
		}
		if (option == 2)
		{
			cart();
		}
		if (option == 3)
		{
			exit(0);
		}
	}
}

void cart()  // Shopping cart
{
	order o;
	int size, choice, serial, ans;
	final:
	float total = 0;
	temp = head.begin();
	system("cls");
	cout << "\tProduct name\t\tPrice\n\n";
	size = head.size();
	for (int i = 0; i<size; i++)
	{
		o = *temp;
		cout << i << "\t";
		cout << o.product_name;
		cout << "\t\t" << o.cost << "\n\n";
		total += o.cost;
		temp++;
	}
	cout << "\nEnter 1 to pay\n\n2.Edit cart\n\n3 to exit\n\n";
	cin >> choice;
	if (choice == 1)
	{
		system("cls");
		cout << "\n\n\n \t\tPRICE= " << total << "\n\n\nTHANK YOU FOR SHOPPING\n\n";
		getch();
	}
	if (choice == 2)
	{
		do
		{
			cout << "\nEnter the serial number of the product to be removed\n";
			cin >> serial;
			if ((serial>size) && (serial<-1))
			{
				cout << "\nInvalid no\n";
				ans = 1;
			}
			else
			{
				temp = head.begin();
				for (int i = 1; i <= serial; i++)
				{
					temp++;
				}
				head.erase(temp);
			}
			cout << "\nEnter 1 to remove more products\n";
			cin >> ans;
		} while (ans == 1);
		goto final;
	}
	if (choice == 3)
	{
		exit(0);
	}
}

void store_product(long int pos_data)  //Enter the data of the purchased items into the database.
{
	int size, t;
	long int offset, prev = 0;
	login d;
	order o;

	ifstream data("data.txt", ios::binary | ios::_Nocreate);
	data.seekg(pos_data, ios::beg);
	data.read((char *)&d, sizeof(d));

	if (d.offset_order == 0)
	{
		fstream data("data.txt", ios::binary | ios::_Nocreate);
		fstream order("order.txt", ios::binary | ios::_Nocreate | ios::ate | ios::in | ios::out);

		data.seekg(pos_data, ios::beg);
		data.read((char *)&d, sizeof(d));
		offset = order.tellg();  //get the address of the order

		d.offset_order = offset;
		data.seekg(pos_data, ios::beg);
		data.write((char *)&d, sizeof(d));
		data.close();  //modified data file with the address of order

		order.seekg(offset, ios::beg);
		prev = order.tellg();  // has the starting address of the order
		feed(prev);
		order.close();
	}
	else  //user has ordered before.link the new items with that.
	{
		offset = d.offset_order;
		while (offset != 0)
		{
			prev = offset;
			fstream order("order.txt", ios::binary | ios::_Nocreate);
			order.seekg(offset, ios::beg);
			order.read((char *)&o, sizeof(o));
			offset = o.offset_same_user;
			order.close();
		}  //loop finds the last order.new order will b chained to the last order found.
		feed(prev); //link the orders
	}
}

void feed(long int prev)
{
	order o;
	int t, size;
	long int offset;

	fstream order("order.txt", ios::in | ios::out | ios::_Nocreate);
	order.seekg(ios::beg);
	order >> t; //read order no
	size = head.size();

	for (int i = 0; i<size; i++)
	{
		temp = head.begin();  //run iterator
		o = *temp;
		fstream ptr("order.txt", ios::binary | ios::_Nocreate | ios::ate | ios::in | ios::out);

		o.order_no = t + i; //store order no in the object
		offset = ptr.tellp();	//store the address
		ptr.write((char *)&o, sizeof(o));  //saved the order in the file
		if (i + 1 != size)
		{
			offset = ptr.tellp();	//store the address
			ptr.seekg(prev, ios::beg);
			ptr.read((char *)&o, sizeof(o));
			o.offset_same_user = offset;   //chained objects
			ptr.seekp(prev, ios::beg);
			ptr.write((char *)&o, sizeof(o));
		}
		head.pop_front();

		ptr.close();
		prev = offset;
	}
	t += size;
	order.seekp(ios::beg);
	order << t; //modified order no
	order.close();
}