/*
------------------------------------------------
Copyright (C) 2015 by Jorge C. Valverde-Rebaza
Email: jvalverr@icmc.usp.br

This file is part of ovLinkCom.

ovLinkCom is a software to calculate communities as groups of links rather than nodes. We consider that
each node belongs to all communities formed by links where that node belongs. Thus, is possible obtain
overlapping node groups. This implementation is based on paper [1].

[1] Yong-Yeol Ahn, James P. Bagrow and Sune Lehmann. "Link communities reveal multiscale complexity in networks".
    Nature 466, pp. 761–764, 2010.

ovLinkCom implementation is based also in the C++ implementation available here: http://barabasilab.neu.edu/projects/linkcommunities/
The main difference of ovLinkCom regarding this implementation is that ovLinCom is easier and faster. ovLinkCom
does not need to do multiple reads of the input file, thus is faster. Also, automatically converts any network format in
the appropriate ovLinkCom format without the need for any additional software. The indices of nodes in output maintain
correspondence with the indices of nodes from input file. Thus, ovLinkCom is easier.


ovLinkCom is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
LPsource is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should
have received a copy of the GNU General Public License along with LPsource. If not, see <http://www.gnu.org/licenses/>.
------------------------------------------------
*/


#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <string.h>

using namespace std;

//global variables
map< int, set<pair<int,int> > > index2cluster;
map< pair<int,int>, map< int,set<pair<int,int> > >::iterator > edge2iter;
vector<int>indexVector;
vector<int>xVector;
vector<int>yVector;


//methods
void clusteringProcess(const char* , double );


int main(int argc, char const *argv[]){
    if (argc != 3){
        cout << "ERROR: something wrong with the inputs" << endl;
        cout << "usage:\n    " << argv[0] << " network.pairs threshold" << endl;
        exit(1);
    }
    double threshold = atof( argv[2] );
    if (threshold < 0.0 || threshold > 1.0){
        cout << "ERROR: specified threshold not in [0,1]" << endl;
        exit(1);
    }

    clusteringProcess( argv[1], threshold );
    return 0;
}



int intersection_size( const set<int> &A, const set<int> &B ) {
    int num = 0;
    set<int>::const_iterator As = A.begin();
    set<int>::const_iterator Af = A.end();
    set<int>::const_iterator Bs = B.begin();
    set<int>::const_iterator Bf = B.end();

    while ( As != Af && Bs != Bf ) {
        if ( *As < *Bs) {
            ++As;
        } else if ( *Bs < *As ) {
            ++Bs;
        } else {
            ++num;
            ++As;
            ++Bs;
        }
    }
    return num;
}

int findInVector(vector<int>listV, int key){
    for(int i = 0; i < listV.size(); i++){
        if(key == listV[i])
            return i;
    }
    return -1;
}

void readNetwork( const char* nameFile ){
    ifstream inFile;
    inFile.open ( nameFile );

    if (!inFile) {
        cout << "\nERROR: unable to open input file" << endl;
        exit(1);
    }

    int ni, nj;
    while (inFile >> ni >> nj){
        if( nj > ni){
            int index_ni = findInVector( indexVector, ni);
            if(index_ni == -1){
                indexVector.push_back( ni );
                index_ni = indexVector.size() - 1;
            }

            int index_nj = findInVector( indexVector, nj);
            if(index_nj == -1){
                indexVector.push_back( nj );
                index_nj = indexVector.size() - 1;
            }

            xVector.push_back( index_ni );
            yVector.push_back( index_nj );
        }
    }

    inFile.close();
}

set<int>* instanceClusters(){
    int num_nodes = indexVector.size();
    set<int> *neighbors = NULL;
    neighbors = new set<int>[num_nodes];

    int ni, nj, index = 0;
    for(int i = 0; i < xVector.size(); i++){
        neighbors[xVector[i]].insert(yVector[i]);
        neighbors[yVector[i]].insert(xVector[i]);

        index2cluster[ index ].insert( make_pair(xVector[i], yVector[i]) ); // build cluster index to set of edge-pairs map
        edge2iter[ make_pair(xVector[i], yVector[i]) ] = index2cluster.find(index); // build edge pair to cluster iter map
        index++;
    }

    for (int i=0; i < num_nodes; i++){
        neighbors[i].insert(i); // neighbors[] is now INCLUSIVE!
    }

    xVector.clear();
    yVector.clear();
    return neighbors;
}

void overlappingLinksClustering(int i0, int i1, int j0, int j1, double jacc, double threshold){
    int idx_i, idx_j;
    map< int, set<pair<int,int> > >::iterator iter_i,iter_j;
    set<pair<int,int> >::iterator iterS;

    if ( jacc >= threshold ) {
        if (i0 >= i1)
            swap(i0,i1); // undirected!

        if (j0 >= j1)
            swap(j0,j1); // undirected!

        iter_i = edge2iter[ make_pair(i0, i1) ];
        iter_j = edge2iter[ make_pair(j0, j1) ];

        if ( iter_i != iter_j ) {
            // always merge smaller cluster into bigger:
            if ( (*iter_j).second.size() > (*iter_i).second.size() ){
                swap(iter_i, iter_j);
            }

            // merge cluster j into i and update index for all elements in j:
            for (iterS = iter_j->second.begin(); iterS != iter_j->second.end(); iterS++){
                iter_i->second.insert( *iterS );
                edge2iter[ *iterS ] = iter_i;
            }
                // delete cluster j:
             index2cluster.erase(iter_j);
        }
    }
}

void calculateJaccard(double threshold){
    set<int>* neighbors = instanceClusters();
    int num_nodes = indexVector.size();

    int n_i, n_j, keystone, len_int;
    double curr_jacc;

    for (int keystone = 0; keystone < num_nodes; keystone++) {
        for (set<int>::iterator i = neighbors[keystone].begin(); i != neighbors[keystone].end(); i++) {
            n_i = *i;

            if (n_i == keystone)
                continue;

            for (set<int>::iterator j = neighbors[keystone].begin(); j != neighbors[keystone].end(); j++ ) {
                n_j = *j;
                if (n_j == keystone or n_i >= n_j)
                    continue;

                len_int = intersection_size( neighbors[n_i], neighbors[n_j] );
                curr_jacc = len_int / (double)( neighbors[n_i].size() + neighbors[n_j].size() - len_int );

                if (keystone < n_i && keystone < n_j){
                    overlappingLinksClustering(keystone, n_i, keystone, n_j, curr_jacc, threshold);
                } else if (keystone < n_i && keystone > n_j){
                    overlappingLinksClustering(keystone, n_i, n_j, keystone, curr_jacc, threshold);
                } else if (keystone > n_i && keystone < n_j){
                    overlappingLinksClustering(n_i, keystone, keystone, n_j, curr_jacc, threshold);
                } else {
                    overlappingLinksClustering(n_i, keystone, n_j, keystone, curr_jacc, threshold);
                }
            }
        }
    }

    delete [] neighbors;
}

void clusteringProcess(const char* nameFile, double threshold){
    set<int> clusterNodes;
    vector<int> groupVector;
    int mc, nc;
    int M = 0, Mns = 0;
    double wSum = 0.0;

    set< pair<int,int> >::iterator S;
    map< int, set<pair<int,int> > >::iterator it;

    char statsAux[500] = "";
    char clustersAux[500] = "";
    char groupsAux[500] = "";
    char infoAux[500] = "";
    strcpy(statsAux,nameFile);
    strcpy(clustersAux,nameFile);
    strcpy(groupsAux,nameFile);
    strcpy(infoAux,nameFile);
    char* statsFile = strtok( statsAux , ".");
    char* clustersFile = strtok( clustersAux , ".");
    char* groupsFile = strtok( groupsAux , ".");
    char* infoFile = strtok( infoAux , ".");
    strcat(statsFile,".stats");
    strcat(clustersFile,".clusters");
    strcat(groupsFile,".groups");
    strcat(infoFile,".info");
    FILE * statsOut     = fopen( statsFile, "w" );
    FILE * clustersOut  = fopen( clustersFile, "w" );
    FILE * groupsOut    = fopen( groupsFile, "w" );
    FILE * infoOut      = fopen( infoFile, "w" );

    readNetwork(nameFile);
    calculateJaccard(threshold);
    for ( it = index2cluster.begin(); it != index2cluster.end(); it++ ) {
        clusterNodes.clear();
        groupVector.clear();
        for (S = it->second.begin(); S != it->second.end(); S++ ){
            fprintf( clustersOut, "%i,%i ", indexVector[S->first], indexVector[S->second] );

            clusterNodes.insert(S->first);
            clusterNodes.insert(S->second);
            groupVector.push_back(indexVector[S->first]);
            groupVector.push_back(indexVector[S->second]);
        }

        mc =   it->second.size();
        nc = clusterNodes.size();
        M += mc;

        if (nc != 2) {
            Mns  += mc;
            wSum += mc * (mc - (nc-1.0)) / ((nc-2.0)*(nc-1.0));
        }
        //remove duplicates
        sort(groupVector.begin() , groupVector.end());
        groupVector.erase(unique(groupVector.begin() , groupVector.end()), groupVector.end());
        for(int i = 0; i < groupVector.size(); i++)
            fprintf( groupsOut, "%i ", groupVector[i]);

        fprintf( groupsOut, "\n");
        fprintf( clustersOut, "\n" );
        fprintf( statsOut, "%i %i\n", mc, nc );
    }

    double density = 2.0 * wSum / M ;
    double notOnesDensity = 2.0 * wSum / Mns;

    fprintf( infoOut, "The partition density is: %f\n", density);
    fprintf( infoOut, "The partition density not counting one-edge clusters: %f\n", notOnesDensity );
    fprintf( infoOut, "Number of clusters: %i\n", index2cluster.size());
    fprintf( infoOut, "Threshold used: %f\n", threshold);

    fclose(clustersOut);
    fclose(groupsOut);
    fclose(statsOut);
    fclose(infoOut);

    index2cluster.clear();
    indexVector.clear();
    edge2iter.clear();
}
