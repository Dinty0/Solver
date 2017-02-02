#include <iostream>
#include <list>
#include <stack>
#include <vector>

using namespace std;

#define QUEEN_NUMBERS 4

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

	for (int i=0; i<QUEEN_NUMBERS; ++i)
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

		while (cpt < QUEEN_NUMBERS && !found)
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

vector<list<int> > prune(/* C,*/ vector< list<int> > node)
{
	// alldifferent()
	int value,ind;
	vector<int> indDomainSize1;
	//unsigned int cpt = 0;

	for (int i=0; i<QUEEN_NUMBERS; ++i)
	{
		if (node.at(i).size()==1)
		{
			indDomainSize1.push_back(i);
		}
	}


	if (indDomainSize1.size() != 0)
	{
		for(unsigned int j=0; j<indDomainSize1.size(); ++j)
		{
			ind = indDomainSize1.at(j);
			value = node.at(ind).front();

			for (int k=0; k<QUEEN_NUMBERS; ++k)
			{
				if (k != ind)
				{	
					node.at(k).remove(value);
				}
			}
		}
	}

	// diagonals

	// for (int l=0; l<QUEEN_NUMBERS; ++l)
	// {
	// 	if (node.at(l).size()==1)
	// 	{
	// 		indDomainSize1.push_back(l);
	// 		cout << "ind :" << l << endl;
	// 	}
	// }




	return node;
}


void BranchAndPrune( /*C,*/ vector<list<int> > node, vector<vector<list<int> > > solutions)
{
	// initialisation de la pile de noeud
	stack<vector<list<int> > > node_stack;
	node_stack.push(node);

	vector<list<int> > E;
	vector<list<int> > F;
	vector<list<int> > G;

	list<int> sd;

	// tant que la pile n'est pas vide
	while(!node_stack.empty())
	{
		E = node_stack.top();
		F = prune(E/*,C*/);

		if (!F.empty())
		{
			if (isSolution(F))
			{
				solutions.push_back(F);
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

	list<int> d1; //{0,1,...QUEEN_NUMBERS-1} Domaine de la variable 1
	list<int> d2; //{0,1,...QUEEN_NUMBERS-1} Domaine de la variable 2
	list<int> d3; //{0,1,...QUEEN_NUMBERS-1} Domaine de la variable 3
	list<int> d4; //{0,1,...QUEEN_NUMBERS-1} Domaine de la variable 4

	list<int> sd;

	for (int i=QUEEN_NUMBERS; i>0; --i)
	{
		d1.push_front(i);
		//d2.push_front(i);
		d3.push_front(i);
		//d4.push_front(i);
		
	}

	d4.push_front(1);
	d2.push_front(2);


	cout << "Display of domains : " << endl;
	printDomain(d1);
	printDomain(d2);
	printDomain(d3);
	printDomain(d4);


	// initialisation du noeud initial 
	vector<list<int> > node;

	node.push_back(d1);
	node.push_back(d2);
	node.push_back(d3);
	node.push_back(d4);

	sd = node.at(smallestDomain(node));

	cout << endl;
	cout << "Display of the smallest domain (of indice :" << smallestDomain(node) << ") : " << endl;
	printDomain(sd);


	node = prune(node);
	cout << endl;
	cout << "Display of node after prune application : " << endl;

	for (int j=0; j<QUEEN_NUMBERS; ++j)
	{
		printDomain(node.at(j));
	}

	//BranchAndPrune(C,node,solutions);

	return 0;


}