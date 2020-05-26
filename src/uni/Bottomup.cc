/*
    This is just a copy of Topdown.cc so that this file can be successfully compiled.
    Implementation of Bottom-up algorithm may be based on this more or less.
 */
extern "C"
{
#include "cx/syscx.h"
}

#include "uniact.hh"
#include "unifile.hh"
#include "../prot-ofile.hh"
#include "../prot-xfile.hh"
#include "../xnsys.hh"

#include "cx/fileb.hh"
#include "cx/tuple.hh"
#include <vector>
#include <set>
#include <queue>

#include <stdio.h>

#include "../namespace.hh"
using std::queue;
using std::set;
using std::vector;

#ifdef DEBUG
void printSquareMatrix(int **matrix, int length)
{
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < length; j++)
        {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
#endif

size_t **allocSquareMatrix(size_t M)
{
    size_t **toReturn = (size_t **)malloc(M * sizeof(*toReturn));
    for (size_t i = 0; i < M; i++)
        toReturn[i] = (size_t *)calloc(M, sizeof(**toReturn));
    return toReturn;
}

size_t **copySquareMatrix(size_t **toCopy, size_t M)
{
    size_t **toReturn = allocSquareMatrix(M);
    for (size_t i = 0; i < M; i++)
        memcpy(toReturn[i], toCopy[i], M * sizeof(**toReturn));
    return toReturn;
}

void freeSquareMatrix(size_t **toFree, size_t M)
{
    for (size_t i = 0; i < M; i++)
        free(toFree[i]);
    free(toFree);
}

//"remove" the node
void clear_cnr(size_t **mat, size_t size, uint i)
{
    for (uint x = 0; x < size; x++)
    {
        for (uint y = 0; y < size; y++)
        {
            if (x == i || y == i)
            {
                mat[x][y] = 0;
            }
        }
    }
}

//get go-to-list
vector<uint> go_to(size_t **mat, size_t size, uint val)
{
    vector<uint> golist;
    for (uint i = 0; i < size; i++)
    {
        if (mat[val][i])
        {
            golist.push_back(i);
        }
    }
    return golist;
}

//check if the node is touched before
bool is_in(vector<std::pair<uint, vector<uint> > > f, uint val, uint idx)
{
    for (uint i = 0; i < idx; ++i)
    {
        if (f[i].first == val)
            return true;
    }
    return false;
}

vector<std::pair<uint, vector<UniAct> > > topdownTreeGenerator(vector<UniStep> legits, size_t M)
{
    size_t gamma = 0;
    size_t c = INT32_MAX; //infinity
    bool isacyclic = false;
    bool gammaExists = false;
    bool isBreak = false;
    size_t **L = NULL;
    size_t **Lprime = NULL;
    uint start = 0;
    vector<std::pair<uint, vector<UniAct> > > actions; //array of <index, actions> root--index=0, nonroot-index=1
    vector<UniAct > root;
    vector<UniAct > nonroot;

    L = allocSquareMatrix(M);
    //first dimention is the x-1 vertex value, second is x vertex valid value

    for (size_t i = 0; i < legits.size(); i++)
        L[legits[i][0]][legits[i][1]] = 1;

    //output the protocol 
    std::cout << "L constructed" << std::endl;
    for (uint i = 0; i < M; i++)
    {
        for (uint j = 0; j < M; j++)
        {
            std::cout << L[i][j] << " ";
        }
        std::cout << std::endl;
    }

    Lprime = copySquareMatrix(L, M); //checking operations and modification are on Lprime

#ifdef DEBUG
    printSquareMatrix(L, M);
#endif

    //check if it's acyclic
    //find all the nodes with 0 incoming edges ;remove the node along with its outgoing edges;
    //repeat until there is no such node left
    bool flag = false;
    do
    {
        flag = false;
        for (uint i = 0; i < M; i++)
        {
            int x_sum = 0;
            int y_sum = 0;
            for (uint j = 0; j < M; j++)
            {
                x_sum += Lprime[j][i];
                y_sum += Lprime[i][j];
            }
            if ((x_sum + y_sum) && (!x_sum || !y_sum)) 
            {
                clear_cnr(Lprime, M, i);
                flag = true;
            }
        }
    } while (flag);

    std::cout << "Lprime :" << std::endl;
    for (uint i = 0; i < M; i++)
    {
        for (uint j = 0; j < M; j++)
        {
            std::cout << Lprime[i][j] << " ";
        }
        std::cout << std::endl;
    }

    isBreak = false;
    for (uint i = 0; i < M && !isBreak; i++)
    {
        for (uint j = 0; j < M; j++)
        {
            if (Lprime[i][j]) //if there are still edges left after modification, then it's cyclic
            {
                isacyclic = false;
                isBreak = true;
            }
        }
    }
    if (isacyclic)
        return actions; //no solution exists

    std::cout << "it is cyclic" << std::endl;

#ifdef DEBUG
    printSquareMatrix(Lprime, M);
#endif

    /*case1: self-loop*/
    //check for self-loop
    for (size_t i = 0; i < M; i++)
    {
        if (Lprime[i][i])
        {
            gamma = i;
            gammaExists = true; //self-loop exists
            break;
        }
    }
    if (gammaExists)
    {
        std::cerr << "gamma is " << gamma << std::endl;
        for (uint i = 0; i < M; i++)
        {
            if (i != gamma) //root actions
                root.push_back(UniAct(0, i, gamma));
                //actions[0].insert(); 
        }
        actions.push_back(std::pair<uint, vector<UniAct > >(0, root));
       // actions[0].assign(root.begin(), root.end());
        std::cout<<"got root actions"<<std::endl;
        for (uint i = 0; i < M; i++)
        {
            for (uint j = 0; j < M; j++)
                if (i == gamma && j != gamma)
                    nonroot.push_back(UniAct(i, j, i));
                    //actions.push_back();
        }
    actions.push_back(std::pair<uint, vector<UniAct > >(1, nonroot));
        //actions[1].assign(nonroot.begin(), nonroot.end());
                std::cout<<"got non-root actions"<<std::endl;

        return actions;
    }

    /*case2: no self-loop but cycle. (so far it is sure to have a cycle)*/
    isBreak = false;
    for (uint i = 0; i < M && !isBreak; i++)
    {
        for (uint j = 0; j < M; j++)
        {
            if (Lprime[i][j]) 
            {
                start = i;
                isBreak = true;
            }
        }
    }
    
    //BFS to find a simple cycle
    isBreak = false;
    bool found = false;
    uint des = 0; //
    vector<uint> golist;
    std::vector<std::pair<uint, vector<uint> > > frontier;
    uint idx = 0;
    vector<uint> ipath;
    ipath.push_back(start);
    frontier.push_back(std::pair<uint, vector<uint> >(start, ipath));
    for (uint idx = 0; !isBreak; ++idx)
    {
        golist = go_to(Lprime, M, frontier[idx].first);
        for (uint i = 0; i < golist.size(); i++)
        {
            vector<uint> temp;
            temp = frontier[idx].second;
            if (!is_in(frontier, golist[i], idx))
            {
                temp.push_back(golist[i]);
                frontier.push_back(std::pair<uint, vector<uint > >(golist[i], temp));
            }
            else if (golist[i] == start)
            {
                found = true;
                des = idx;
                isBreak = true;
            }
        }
    }
    uint *permufun = new uint[M];
    if (found)
    {
        for (uint i = 0; i < M; i++)
        {
            permufun[i] = INT32_MAX; //use Infinity to mark that it doesn't point to any value cuz 0 is a possible value
        }
        uint k = 0;
        while( k < frontier[des].second.size()-1 ){
            permufun[frontier[des].second[k]] = frontier[des].second[k + 1];
            k++;
        }
        permufun[frontier[des].second[k]]=frontier[des].second[0];
        for (uint i = 0; i < M; i++)
        {
            std::cout<<permufun[i]<<" ";
        }
        std::cout<<std::endl;
        for (uint i = 0; i < M; i++)
        {
            if (permufun[i] != (INT32_MAX))
            {
                c = i; //get c0
                break;
            }
        }
        //build path to some vertex (c0) in the cycle
        for(uint i=0;i<M;i++)
        {
            if(permufun[i]==INT32_MAX)
            {
                permufun[i]=c;
            }
        }
        for (uint i = 0; i < M; i++)
        {
            if (i != c)
            {
                root.push_back(UniAct(0, i, c));
                
            }
        }
        actions.push_back(std::pair<uint, vector<UniAct > >(0, root));
     
                std::cout<<"got root actions"<<std::endl;

        for (uint i = 0; i < M; i++)
            for (uint k = 0; k < M; k++)
                if (permufun[i] != (INT32_MAX) && k != permufun[i])
                nonroot.push_back(UniAct(i, k, permufun[i]));
                
        actions.push_back(std::pair<uint, vector<UniAct > >(1, nonroot));
                std::cout<<"got non-root actions"<<std::endl;

    }


    freeSquareMatrix(L, M);
    L = NULL;
    freeSquareMatrix(Lprime, M);
    Lprime = NULL;
    delete[] permufun;

    return actions;
}

uint ReadTDTree(const char *filepath, Xn::Sys &sys, vector<UniStep> &legits);

/** Execute me now!**/
int main(int argc, char **argv)
{
    int argi = init_sysCx(&argc, &argv);

    if (argi + 1 > argc)
        failout_sysCx("Need at least one argument (an input file).");

    const char *in_filepath = argv[argi++];
    Xn::Sys sys; /// For I/O only.
    vector<UniStep> legits;
    //legits.clear();
    /*uint domsz = ReadTDTree(in_filepath, sys, legits);
    if (domsz == 0)
        failout_sysCx(in_filepath);*/

    //--------hard coded cases
    uint domsz = 0;
    /* 2-coloring*/
    domsz = 2;
    legits.push_back(UniStep(0, 1));
    legits.push_back(UniStep(1, 0));

    /*tExample#1
    domsz = 5;
    legits.push_back(UniStep(2, 1));
    legits.push_back(UniStep(3, 2));
    legits.push_back(UniStep(1, 3));
    legits.push_back(UniStep(3, 4));
    legits.push_back(UniStep(0, 1));
    */

    //Example#2 
    /* domsz = 5;
    legits.push_back(UniStep(0, 2));
    legits.push_back(UniStep(1, 0));
    legits.push_back(UniStep(2, 1));
    legits.push_back(UniStep(3, 1));
    legits.push_back(UniStep(2, 4));
    legits.push_back(UniStep(4, 3));*/
    

    //Example#3
    /*domsz = 5;
    legits.push_back(UniStep(2, 1));
    legits.push_back(UniStep(3, 1));
    legits.push_back(UniStep(3, 2));
    legits.push_back(UniStep(1, 3));
    legits.push_back(UniStep(3, 4));
    legits.push_back(UniStep(0, 1));
    */

 

    /* Broadcast
    legits.push_back(UniStep(0,0));
    legits.push_back(UniStep(1,1));
    */
    //hard-coded cases

    // (Debugging) Output all the legitimate readable states.
    printf("Legitimate states for non-root process P[i]:\n");
    for (uint i = 0; i < legits.size(); ++i)
    {
        printf("x[parent]==%u && x[i]==%u\n", legits[i][0], legits[i][1]);
    }

    vector<std::pair<uint, vector<UniAct> > > actions;
    std::cout << domsz << std::endl;
    ////////////////////////////////////////////////////////////////////////
    actions = topdownTreeGenerator(legits, domsz);
    ////////////////////////////////////////////////////////////////////////

    // (Debugging) Output all the synthesized acctions.
    printf("Synthesized actions for root process:\n");
    for(uint i=0; i<actions[0].second.size(); i++)
    {
        printf("x[root]=%u --> x[root]=%u\n", actions[0].second[i][1], actions[0].second[i][2]);
    }

    printf("Synthesized actions for non-root process P[i]:\n");
    for (uint i = 0; i < actions[1].second.size(); ++i)
    {
        printf("x[parent]==%u && x[i]==%u --> x[i]:=%u\n", actions[1].second[i][0], actions[1].second[i][1], actions[1].second[i][2]);
    }

    const char *out_filepath = argv[argi];
    if (out_filepath)
    {
        ++argi;
        //oput_protocon(out_filepath, Table<UniAct>(actions));
    }

    lose_sysCx();
    return 0;
}

/** Read a topdown tree specification.**/
uint ReadTDTree(const char *filepath, Xn::Sys &sys, vector<UniStep> &legits)
{
    legits.clear();
    sys.topology.lightweight = true;
    ProtoconFileOpt infile_opt;
    infile_opt.text.moveq(textfile_AlphaTab(0, filepath));
    if (!ReadProtoconFile(sys, infile_opt))
        BailOut(0, "Cannot read file!");
    const Xn::Net &topo = sys.topology;

    // Do some ad-hoc checking that this is a unidirectional ring.
    if (sys.direct_pfmla.sat_ck())
        BailOut(0, "Should not have actions.");
    if (topo.pc_symms.sz() != 1)
    {
        DBog_luint(topo.pc_symms.sz());
        BailOut(0, "Should have only one kind of process.");
    }
    if (topo.pcs.sz() < 2)
        BailOut(0, "Should have more than 1 process.");

    // Ensure the invariant is given inside the process definition.
    {
        P::Fmla invariant(true);
        for (uint i = 0; i < topo.pcs.sz(); ++i)
        {
            invariant &= topo.pcs[i].invariant;
        }
        if (!invariant.equiv_ck(sys.invariant))
            BailOut(0, "Please specify invariant in process definition.");
    }

    // Just look at process P[0].
    const Xn::PcSymm &pc_symm = topo.pc_symms[0];
    const Xn::Pc &pc = *pc_symm.membs[0];
    if (pc.wvbls.sz() != 1)
        BailOut(0, "Should write 1 variable.");
    if (pc.rvbls.sz() != 2)
        BailOut(0, "Should read-only 1 variable.");

    // Get references to this process's variables that can be
    // used in the context of the predicate formulas.
    Table<uint> rvbl_indices(2);
    rvbl_indices[0] = pc.rvbls[0]->pfmla_idx;
    rvbl_indices[1] = pc.rvbls[1]->pfmla_idx;
    if (pc_symm.spec->wmap[0] == 0)
        SwapT(uint, rvbl_indices[0], rvbl_indices[1]);

    // Get all legitimate states.
    P::Fmla legit_pf = pc.invariant;
    while (legit_pf.sat_ck())
    {
        Table<uint> state(2);

        // Find a readable state of this process that fits the legitimate states.
        legit_pf.state(&state[0], rvbl_indices);

        // Remove the corresponding predicate formula from {legit_pf}.
        legit_pf -= topo.pfmla_ctx.pfmla_of_state(&state[0], rvbl_indices);

        legits.push_back(UniStep(state[0], state[1]));
    }

    return topo.vbl_symms[0].domsz;
}

END_NAMESPACE

int main(int argc, char **argv)
{
    return PROJECT_NAMESPACE::main(argc, argv);
}
