#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <cmath>
#include <unistd.h>
#include <ctime>
#include <string.h>
#include <curses.h>

using namespace std;

 #define SIZE 4 // 5,6, .. 8

//################################################################################
//################### USEFUL METHODS #############################################
//################################################################################
void printDomain(list<int> domain)
{
	unsigned int cpt = 1;

	cout << "{";
	for (list<int>::iterator it=domain.begin(); it!=domain.end(); ++it)
	{
		if (cpt < domain.size())
		{
			cout << *it << ",";
			cpt++;
		}
		else
		{
			cout << *it << "}";
		}
	}
}

bool isSolution(vector<list<int> > node)
{
	bool res = true;

	for (int i=0; i<SIZE*SIZE; ++i)
	{
		if (node.at(i).size() != 1)
		{
			res = false;
		}
	}

	return res;
}

int smallestDomain(vector<list<int> > node)
{
	bool found = false;
	unsigned int minsize = 2;
	unsigned int cpt = 0;
	int ind = -1;

	while (!found && minsize)
	{

		while (cpt < SIZE*SIZE && !found)
		{
			if (node.at(cpt).size() == minsize)
			{
				found = true;
				ind = cpt;
			}

			++cpt;
		}

		cpt = 0;
		++minsize;
	}

	return ind;
}
//################################################################################
//################################################################################

//################################################################################
//################### PROTO PRUNE / SIMPLE BACKTRACKING ##########################
//################################################################################

bool constraintsLatinSquare(vector< pair <int,int> >  indval)
{
	bool res = true; 

	for (unsigned int i=0; i<indval.size(); ++i)
	{

		for (unsigned int j=i+1; j<indval.size(); ++j)
		{
			if (indval.at(i).second == indval.at(j).second && (j-i<SIZE-(i%SIZE)))
			{
				res = false;
			}

			if (indval.at(i).second == indval.at(j).second && ( (i%SIZE) == (j%SIZE) ))
			{
				res = false;
			}
		}
	}

	return res;
}

vector< list<int> > backtracking(vector< list<int> > node/*, Constraints* c*/)
{
	vector< pair<int,int> > size1; //stocks the domains which size is 1

	for (unsigned int i=0; i <node.size(); ++i)
	{
		if (node.at(i).size() == 1)
		{
			size1.push_back(make_pair(i,node.at(i).front()));
		}
	}


	if (size1.size() >= 2)
	{
		if (/*c->*/constraintsLatinSquare(size1))
		{
			return node;
		}
		else 
		{
			vector< list<int> > emptyList;
			return emptyList;
		}
	}

	return node;
}
//################################################################################
//################################################################################

//################################################################################
//################### PRUNE LOOK AHEAD / FORWARD CHECKING ########################
//################################################################################
vector<list<int> > pruningLookAhead(/* C,*/ vector< list<int> > node)
{
	// Pruning look ahead
	int value,ind;
	vector<int> indDomainSize1; // Domains wich size = 1

	// Stocks domains of size 1
	for (int i=0; i<SIZE*SIZE; ++i)
	{
		if (node.at(i).size()==1)
		{
			indDomainSize1.push_back(i);
		}
	}

	// If there are 1 or more domains of size 1, prune
	if (indDomainSize1.size() != 0)
	{
		for(unsigned int j=0; j<indDomainSize1.size(); ++j)
		{
			// Get the only value of the domain
			ind = indDomainSize1.at(j);
			value = node.at(ind).front();

			if (value <= SIZE && value > 0)
			{
				// Remove the value in the other domains
				for (int k=0; k<SIZE*SIZE; ++k)
				{
					if (k != ind)
					{	

						if (abs(ind-k)<SIZE-(ind%SIZE) && ind<k)
						{
							node.at(k).remove(value);
						}

						if (abs(ind-k)<SIZE-(k%SIZE) && ind>k)
						{
							node.at(k).remove(value);
						}

						if ((ind%SIZE) == (k%SIZE))
						{
							node.at(k).remove(value);
						}
					}
				}	
			}
		}
	}

	// FINAL VERIFICATION
	// If the last removal put all domain at size 1, it will be considered as a solution otherwise
	int cpt=0;
	for(unsigned int k=0; k<node.size(); ++k)
	{
		if (node.at(k).size() == 1)
		{
			cpt++;
		}
	}

	if (cpt == SIZE*SIZE)
	{
		for(int j=0; j<(int)node.size(); ++j)
		{
			// Get the only value of the domain
			value = node.at(j).front();

			if (value <= SIZE && value > 0)
			{
				// Remove the value in the other domains
				for (int k=0; k<SIZE*SIZE; ++k)
				{
					if (k != j)
					{	
						if (abs(j-k)<SIZE-(j%SIZE) && j<k)
						{
							node.at(k).remove(value);
						}

						if (abs(j-k)<SIZE-(k%SIZE) && j>k)
						{
							node.at(k).remove(value);
						}

						if ((j%SIZE) == (k%SIZE))
						{
							node.at(k).remove(value);
						}
					}
				}
			}
		}
	}

	// If one of the domains is empty, return an empty node to discard it
	for(unsigned int k=0; k<node.size(); ++k)
	{
		if (node.at(k).size() == 0)
		{
			vector< list<int> > emptyList;
			node = emptyList;
		}
	}
	
	return node;
}
//################################################################################
//################################################################################

//################################################################################
//####################### AC BASED PRUNING #######################################
//################################################################################
bool HasSupport(/* C,*/ int a, list<int> D, int i, int j)
{
	bool hasSup = false;

	for (list<int>::iterator it=D.begin(); it!=D.end(); ++it)
	{
		if (i<j)
		{
			if ( (abs(j-i)>=SIZE-(i%SIZE) && ((j%SIZE) != (i%SIZE))) )
			{
				hasSup = true;
			}
			else
			{
				if ( a != *it)
				{
					hasSup = true;
				}
			} 
		}
		else if (i>j)
		{
			if ( (abs(j-i)>=SIZE-(j%SIZE)) && ((j%SIZE) != (i%SIZE)) )
			{
				hasSup = true;
			}
			else
			{
				if ( a != *it)
				{
					hasSup = true;
				}
			} 
		}
		else
		{
			hasSup = true;
		}
	}

	return hasSup;
}


vector<list<int> > ACBasedPruning(/* C,*/ vector< list<int> > node)
{
	// Arc consistency based pruning

	for (unsigned int j=0; j<node.size(); ++j)
	{
		for (int i=0; i<SIZE*SIZE; ++i)
		{
			list<int> D = node.at(i);

			for (list<int>::iterator it=D.begin(); it!=D.end(); ++it)
			{
				if (!HasSupport(*it,node.at(j),j,i))
				{
					node.at(i).remove(*it);
				}
			}
		}
	}

	int cpt=0;
	for(unsigned int k=0; k<node.size(); ++k)
	{
		if (node.at(k).size() == 1)
		{
			cpt++;
		}
	}

	if (cpt == SIZE*SIZE)
	{
		for (unsigned int j=0; j<node.size(); ++j)
		{
			for (int i=0; i<SIZE*SIZE; ++i)
			{
				list<int> D = node.at(i);

				for (list<int>::iterator it=D.begin(); it!=D.end(); ++it)
				{
					if (!HasSupport(*it,node.at(j),i,j))
					{
						node.at(i).remove(*it);
					}
				}
			}
		}
	}


	for(unsigned int k=0; k<node.size(); ++k)
	{	
		if (node.at(k).size() == 0)
		{
			vector< list<int> > emptyList;
			node = emptyList;
		}
	}
	
	return node;
}
//################################################################################
//################################################################################

void BranchAndPrune(/*Constraints* c,*/ vector<list<int> > node, vector<vector<list<int> > > *solutions, char choice)
{
	// initialisation de la pile de noeud
	stack<vector<list<int> > > node_stack;
	node_stack.push(node);

	vector< list<int> > E;
	vector< list<int> > F;
	vector< list<int> > G;

	list<int> sd;

	// tant que la pile n'est pas vide
	while(!node_stack.empty())
	{
		E = node_stack.top();
		node_stack.pop();

		if (choice == '3')
		{
			F = ACBasedPruning(E/*,c*/);
		}
		else if (choice == '2')
		{
			F = pruningLookAhead(E/*,c*/);
		}
		else if (choice == '1')
		{
			F = backtracking(E/*,c*/);
		}
		
		if (!F.empty())
		{
			if (isSolution(F))
			{
				solutions->push_back(F);
			}
			else
			{
				int indsd = smallestDomain(F);
				sd = F.at(indsd);

				for (int v : sd)
				{
					G = F;
					list<int> domain_split;
					domain_split.push_back(v);
					G.at(indsd) = domain_split;

					node_stack.push(G);
				}
			}
		}
	}
}


int main()
{

	// initialisation des domaines
	vector< list<int> > InitialDomains;

	for (int i=0; i<SIZE*SIZE; ++i)
	{
		list<int> domain;

		for (int j=SIZE; j>0; --j)
		{
			domain.push_front(j);
		}

		InitialDomains.push_back(domain);
	}

	cout << endl;
	cout << "--------------------------------" << endl;
	cout << "Display of domains : " << endl;
	cout << endl;

	for (int i=0; i<SIZE*SIZE; ++i)
	{
		cout << i << ") "; printDomain(InitialDomains.at(i));
		cout << endl;
	}
	cout << "--------------------------------" << endl;
	cout << endl;

	vector< vector< list<int> > > solutions;
	char choice = '0';
	cout << "Choose the pruning algorithm among this list (enter associate integer) : \n\n 1) Backtracking\n 2) Look Ahead\n 3) Arc consistency based pruning" << endl;
	cin >> choice;

	while (choice != '1' && choice != '2' && choice != '3')
	{
		cout << "Error, enter \"1\",\"2\" or \"3\" please : \n 1) Backtracking\n 2) Look Ahead\n 3) Arc consistency based pruning" << endl;
		cin >> choice;
	}

	string nameAlgo;

	if (choice == '1')
	{
		nameAlgo = "'Backtracking' algorithm";
	}
	else if (choice == '2')
	{
		nameAlgo = "'Look Ahead' algorithm'";
	}
	else if (choice == '3')
	{
		nameAlgo = "'Arc consistency' based pruning";
	}

	cout << endl;
	cout << "Test of BranchAndPrune with " << nameAlgo << " : " << endl << endl;

	double duration;
	std::clock_t start; 

	start = std::clock(); // START OF THE TIMER
	BranchAndPrune(/*c,*/InitialDomains,&solutions,choice);
    duration = (double) ( std::clock() - start ) / (double) CLOCKS_PER_SEC; // END OF THE TIMER

 	cout << "Solutions found : " << solutions.size() << endl;

    cout << "Display of the solutions" << endl;
    cout << "--------------------------------" << endl;

	if (solutions.size() > 0)
	{
		unsigned int j = 0;
		char keyIN; 
		while (keyIN != 'q' && j<solutions.size())
		{
			cout << endl;
			cout << "- Solution " << j << " : (USE 'q' TO QUIT) " << endl;
			for(unsigned int k=0; k<solutions.at(j).size(); ++k)
			{
				if (k%SIZE == 0) {cout << endl;}
				printDomain(solutions.at(j).at(k));
			}
			cout << endl;
			cin >> keyIN;
			j++;
		}
	}

	cout << endl;
	cout << "TOTAL DURATION : " << duration << "s." << endl;

	return 0;
} 