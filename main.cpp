///lots of stuff from https://www.geeksforgeeks.org/fibonacci-heap-deletion-extract-min-and-decrease-key/

#include <bits/stdc++.h>

using namespace std;


struct Node
{
    int data, degree;
    Node *parent = NULL, *child = NULL, *left = NULL, *right = NULL;

};

///pentru a insera un nou nod, trebuie sa:
/// 1 -> cream noul nod
/// 2 -> verificam daca heapul este sau nu gol
/// 3 -> daca este gol, setam minimul in nodul nou inserat
/// daca nu este gol, updatam minimul daca este cazul

void fibonacciInsert(Node*&Min, int new_data, int&n)
{

    n++;
    Node *new_node = new Node;
    new_node->data = new_data;
    new_node->degree = 0;
    new_node->parent = NULL;
    new_node->child = NULL;
    if(Min == NULL) ///verificam daca heapul este sau nu gol
    {

        new_node->left = new_node;
        new_node->right = new_node;

        ///setam pointerul Min catre nod
        Min = new_node;
        return;
    }
    new_node->left = Min->left;
    Min->left->right = new_node;
    Min->left = new_node;
    new_node->right = Min;

    if(new_node->data < Min->data)
        Min = new_node;

}
void FibonacciHeap(Node*&Min, int&n)
{
    n = 0;
    Min = NULL;
}
Node* fibonacciBuildHeap(vector<int> v)
{
    Node *Min;
    int n = 0;
    FibonacciHeap(Min,n);
    for(auto &x: v)
        fibonacciInsert(Min,x,n);
    return Min;

}
int findMin(Node*Min)
{
    return Min->data;
}
void fibonacciDisplayChildren(Node*x)
{
    if(x->child != NULL)
    {

        cout<<"( Copii nodului "<<x->data<<" sunt : ";
        Node *it = x->child;
        do
        {
            if(it != x->child)cout<<", ";
            cout<<it->data;

            fibonacciDisplayChildren(it);
            it = it->right;

        }
        while(it != x->child);
        cout<<".)\n";
    }


}
void fibonacciDisplay(Node *Min, int n=0)
{
    Node*start = Min;
    if(start == NULL)
    {
        cout<<"Heap-ul este gol\n";
        return;
    }
    cout<<"Heapul are "<<n<<" noduri si valorile: \n";
    do
    {
        if (start != Min)
        {
            cout << "-->";
        }
        cout<<start->data<<" ";
        fibonacciDisplayChildren(start);

        start = start->right;

    }
    while(start != Min);

    cout<<"\n";
}


///linkuim nodurile heapului in relatie parinte-copil
void fibonacciLink(Node *&x, Node*&y)
{


    Node *new_child = y;

    y->left->right = y->right;
    y->right->left = y->left;
    x->degree++;

    new_child->left = new_child;
    new_child->right = new_child;

    ///salvam copilul lui x ca sa ne fie mai usor sa implementam legaturile
    Node*brother = x->child;
    if(brother != NULL)
    {
        ///inseram y in dreapta copilului lui x
        new_child->right = brother->right;
        brother->right->left = new_child;
        brother->right = new_child;
        new_child->left = brother;

        ///il punem pe x sa pointeze catre cel mai mic copil
        if(new_child->data < brother->data)
            x->child = new_child;

    }
    else
        x->child = new_child;

    ///il facem pe x parinte a lui new_child
    new_child->parent = x;

}
void add(Node*&Min,Node*new_node)
{

    if(Min != NULL)
    {

        Min->right->left = new_node;
        new_node->right = Min->right;
        Min->right = new_node;
        new_node->left = Min;
        return;
    }

    Min = new_node;
    Min->right = Min;
    Min->left = Min;
}

void consolidate(Node*&Min, int n)
{

    int D = log2(n);
    Node* A[D+1];
    for(int i = 0; i <= D+1; i++)
        A[i] = NULL;

    Node *it = Min;
    Node *x = it;
    do
    {

        int d = x->degree;
        while(A[d] != NULL)
        {

            ///daca mai exista un nod de acelasi grad, trebuie sa facem link intre cele doua
            Node *y = A[d];
            ///interschimbam pointerii; astfel x
            ///va pointa mereu catre nodul care are valoare retinuta
            ///mai mica - cel care va fi parintele celuilalt nod de acelasi grad
            if(x->data > y->data)
            {

                Node *aux = x;
                x = y;
                y = aux;
            }
            if(y == Min)
                Min = x;

            fibonacciLink(x,y);
            A[d] = NULL;
            d = d+1;

        }
        A[d] = x;
        x = x->right;
    }
    while(x != Min);

    ///golim heapul fara a sterge nodurile din memorie
    ///vom reconstrui heapul cu ajutorul vectorului A
    Min = NULL;
    for(int i=0; i <= D+1; i++)
    {

        if(A[i] != NULL)
        {

            add(Min, A[i]);
            ///actualizam minimul
            if(Min == NULL || A[i]->data < Min->data)
                Min = A[i];
        }
    }

}
void fibonacciUnion(Node *&Min1,Node*&Min2, Node *&Min, int n1, int n2, int &n)
{
    FibonacciHeap(Min,n); ///cream un nou heap H
    Min = Min1;           ///acum heap-ul nostru contine componentele heapului H1
    ///vom concatena lista nodurilor lui H2 cu cele ale lui H
    ///folosim 2 pointeri Node temporari
    Node *last,*last2;
    last = Min->left;
    last2 = Min2->left;

    Min->left->right = Min2;
    Min2->left->right = Min;
    Min->left = Min2->left;
    last2 = last->right;
    n = n1+n2;

    if(Min1 == NULL || (Min2 != NULL && Min2->data < Min1->data))
        Min = Min2;
}


Node* fibonacciExtractMin(Node*&Min, int &n)
{

    Node *kid = NULL, *temp, *brother = NULL;
    temp = Min;
    if(Min != NULL)
    {

        /// O sa folosim 3 pointeri de tip Node
        /// 1-> cel cu care sa prelucram copiii node - denumit kid
        /// 2-> unul care sa retina adresa fratelui nodului, inainte sa modificam legaturile nodului (si care face posibila iteratia prin child list) - denumit brother
        /// 3-> unul care sa retina adresa nodului Min, cel care ne va ajuta sa eliminam nodul minim din root list - denumit temp
        if(temp->child != NULL)
        {
            ///pentru a-l scoate pe temp din root list, adaugam fiecare copil al lui in root list
            Node *kid = temp->child;
            do
            {
                brother = kid->right;
                temp->left->right = kid;
                kid->left = temp->left;
                temp->left = kid;
                kid->right = temp;
                kid->parent = NULL;
                kid = brother;

            }
            while(brother != temp->child);

        }

        temp->left->right = temp->right;
        temp->right->left = temp->left;

        ///daca heapul avea singur nod
        if(temp == temp->right)
        {
            Min = NULL;
        }
        else
        {
            Min = temp->right;
            temp->right = temp;
            temp->left = temp;

            consolidate(Min, n);
        }
        n--;

    }
    return temp;
}


int main()
{
    int option, run = 1, k = 0, n;
    vector <Node*> heaps;
    int sizes[1000];


    while (run)
    {
        cout<<"Itroduceti optiunea: 1 = build heap \n 2 = inserare intr-un heap existent \n 3 = gaseste min \n 4 = sterge min \n 5 = merge 2 heaps \n";
        cin>>option;
        switch (option)
        {
            case 1:
                {
                    cout<<"Dati un numar de elemente si valorile acestor elemente: \n";
                    int nr, el;
                    Node *h;
                    vector <int> v;
                    cin>>nr;
                    for(int i = 0; i < nr; i++)
                    {
                        cin>>el;
                        v.push_back(el);
                    }

                    h = fibonacciBuildHeap(v);
                    heaps.push_back(h);
                    sizes[k] = nr;
                    k++;
                    fibonacciDisplay(h, nr);
                    break;
                }
            case 2:
                {
                    cout<<"Introduceti valoarea pe care doriti s-o inserati: ";
                    int val;
                    cin>>val;
                    cout<<"\nIntroduceti indexul heapului in care doriti sa inserati: ";
                    int idx;
                    cin>>idx;
                    fibonacciInsert(heaps[idx], val, sizes[idx]);
                    fibonacciDisplay(heaps[idx], sizes[idx]);
                    break;

                }
            case 3:
                {
                    cout<<"\nIntroduceti indexul heapului in care doriti sa cautati minimul: ";
                    int idx2;
                    cin>>idx2;
                    cout<<"\nMinimul este: "<<findMin(heaps[idx2])<<"\n";
                    break;
                }
            case 4:
                {
                    cout<<"\nIntroduceti indexul heapului din care doriti sa extrageti minimul: ";
                    int idx3;
                    cin>>idx3;
                    Node *result = fibonacciExtractMin(heaps[idx3], sizes[idx3]);
                    cout<<"Nodul "<<result->data<<" a fost sters!\n";
                    delete result;
                    //sizes[idx3]-=1;
                    fibonacciDisplay(heaps[idx3], sizes[idx3]);
                    break;

                }
            case 5:
                {
                    cout<<"\nIntroduceti indexul primului heap: ";
                    int idx4;
                    cin>>idx4;

                    cout<<"\nIntroduceti indexul celui de-al doilea heap: ";
                    int idx5;
                    cin>>idx5;

                    Node *mergedHeaps;
                    int mergedSize;

                    fibonacciUnion(heaps[idx4], heaps[idx5], mergedHeaps, sizes[idx4], sizes[idx5], mergedSize);
                    fibonacciDisplay(mergedHeaps, mergedSize);
                    heaps.push_back(mergedHeaps);
                    sizes[k] = mergedSize;
                    k++;
                    break;
                }
            default:
                cout<<"Optiune nevalida!";
        }

        cout<<"Continuati? 1/0: ";
        cin>>run;
    }

    return 0;
}
