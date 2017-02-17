#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <cmath>

//#include "queensConstraints.cpp"

using namespace std;

#define QUEENS_NUMBER 10 // 5,6,...,12

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
			cout << *it << "}" << endl;
		}
	}
}


bool isSolution(vector<list<int> > node)
{
	bool res = true;

	for (int i=0; i<QUEENS_NUMBER; ++i)
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

		while (cpt < QUEENS_NUMBER && !found)
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

bool constraintsQueen(vector< pair <int,int> >  indval)
{
	bool res = true; 

	for (unsigned int i=0; i<indval.size(); ++i)
	{
		for (unsigned int j=i+1; j<indval.size(); ++j)
		{
			if (indval.at(i).second == indval.at(j).second)
			{
				res = false;
			}

			int resval = abs(indval.at(i).second - indval.at(j).second);
			int resind = indval.at(j).first - indval.at(i).first;

			if (resval == resind)
			{
				res = false;
			}
		}
	}

	return res;
}

vector< list<int> > protoPrune (vector< list<int> > node/*, Constraints* c*/)
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
		if (/*c->*/constraintsQueen(size1)) //remplacer par verify constraints
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

void BranchAndPrune(/*Constraints* c,*/ vector<list<int> > node, vector<vector<list<int> > > *solutions)
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
		F = protoPrune(E/*,c*/);

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

	list<int> d1; //{0,1,...QUEENS_NUMBER-1} Domaine de la variable 1
	list<int> d2; //{0,1,...QUEENS_NUMBER-1} Domaine de la variable 2
	list<int> d3; //{0,1,...QUEENS_NUMBER-1} Domaine de la variable 3
	list<int> d4; //{0,1,...QUEENS_NUMBER-1} Domaine de la variable 4
	list<int> d5;
	list<int> d6;
	list<int> d7;
	list<int> d8;
	list<int> d9;
	list<int> d10;
	// list<int> d11;
	// list<int> d12;

	list<int> sd;

	for (int i=QUEENS_NUMBER; i>0; --i)
	{
		d1.push_front(i);
		d2.push_front(i);
		d3.push_front(i);
		d4.push_front(i);
		d5.push_front(i);
		d6.push_front(i);
		d7.push_front(i);
		d8.push_front(i);
		d9.push_front(i);
		d10.push_front(i);
		// d11.push_front(i);
		// d12.push_front(i);
	}

	cout << endl;
	cout << "Display of domains : " << endl;
	printDomain(d1);
	printDomain(d2);
	printDomain(d3);
	printDomain(d4);
	printDomain(d5);
	printDomain(d6);
	printDomain(d7);
	printDomain(d8);
	printDomain(d9);
	printDomain(d10);
	// printDomain(d11);
	// printDomain(d12);

	// initialisation du noeud initial 
	vector<list<int> > node;

	node.push_back(d1);
	node.push_back(d2);
	node.push_back(d3);
	node.push_back(d4);
	node.push_back(d5);
	node.push_back(d6);
	node.push_back(d7);	
	node.push_back(d8);
	node.push_back(d9);
	node.push_back(d10);
	// node.push_back(d11);
	// node.push_back(d12);

	sd = node.at(smallestDomain(node));

	// Test d'implémentation du pattern strategy (a venir)
	// cout << "Construction of contraints : " << endl;
	// Constraints* c = new queensConstraints();

	cout << endl;
	cout << "Display of the smallest domain (of indice :" << smallestDomain(node) << ") : " << endl;
	printDomain(sd);

	cout << endl;
	cout << "Test of BranchAndPrune with protoPrune : " << endl;

	vector< vector< list<int> > > solutions;
	BranchAndPrune(/*c,*/node,&solutions);

	if (solutions.size() > 0)
	{
		cout << "There are " << solutions.size() << " solutions." << endl;

		for (unsigned int j=0; j<solutions.size(); ++j)
		{
			cout << "    Solution " << j << " : " << endl;
			for(unsigned int k=0; k<solutions.at(j).size(); ++k)
			{
				printDomain(solutions.at(j).at(k));
			}
		}
	}

	return 0;
}