#include <iostream>
#include <fstream>
#include <cmath> 
using namespace std;

    struct xyCoord{
		double xCoord,yCoord;
		xyCoord(){} 
		xyCoord(int a, int b){
			xCoord=a;
			yCoord=b;
		}		
	};	
	
	class Point{
		public:
		int xCoord,yCoord,Label;
		Point(){}
		
		Point(int a, int b,int c) { 
		      xCoord=a; 
			  yCoord=b;
			  Label=c;
		}
	};
	
	class Kmean{
		public:
	    int  kCluster,numPts;
		int  changeFlag;
		xyCoord*kCentroids;	      
		Point*ptList; 
		 
		Kmean(int k,int n){
		    kCluster=k;
			numPts=n;	  
        }//constructor
          
		void loadPtList(istream & inFile){
			ptList= new Point[numPts];			 
			for(int i=0;i<numPts;i++){
			 	Point a;
			 	inFile>>a.xCoord>>a.yCoord;
			 	ptList[i]=a;
			}	  	
	    }//loadPtList
		  
		void assignLable (){
		  	for(int i=0;i<numPts;i++){		  		
		  	    ptList[i].Label=(i%kCluster)+1;
			}
	    }//assignLable
		  
		void  computeCentroid(int kCluster){
		    	int*sumCnt; 
		    	double*sumX;
		    	double*sumY;
		    	
		    	sumCnt = new int[kCluster+1]; 
		    	sumX = new double[kCluster+1];
		    	sumY = new double[kCluster+1];
		    	
			    for(int i=0;i<kCluster+1;i++){
			  	    sumCnt[i]=0;
			    	sumX[i]=0.0;
			  	    sumY[i]=0.0;
			    }
			  
			    int ptIndex=0;
			
		    	while(ptIndex < numPts){			
			         int label= ptList[ptIndex].Label;
		       	     sumX [label] += ptList[ptIndex].xCoord;
                     sumY [label] += ptList[ptIndex].yCoord;
                     sumCnt [label]++;
                     ptIndex ++;
		        }
		
		        
		    	int clusterIndex=1;
			    kCentroids= new xyCoord[kCluster+1];
			    
		    	while(clusterIndex <= kCluster){				 
		             kCentroids[clusterIndex].xCoord = sumX[clusterIndex]/sumCnt[clusterIndex];
		             kCentroids[clusterIndex].yCoord = sumY[clusterIndex]/sumCnt[clusterIndex];
                     clusterIndex++;
		        }
		    
		}//computeCentroid
		  		  		  
		void clusterCheck (int kCluster){
		  	double* dist;
		  	dist = new double[kCluster +1];
		  	for(int a=0;a<kCluster +1;a++){
		  		dist[a]=0.0;
			}
			
			int ptIndex=0;
			
			while(ptIndex < numPts){
			
		    	int minLabel=ptList[ptIndex].Label;
			    double minDist=99999.0;
			
		    	int clusterIndex=1;						
			
			    while(clusterIndex <= kCluster){
				      dist[clusterIndex] = computeDist (ptList[ptIndex], kCentroids[clusterIndex]);
	                  if (dist[clusterIndex]<minDist){
	                      minDist=dist [clusterIndex];
	    	       	      minLabel=clusterIndex;
	                  }
    			      clusterIndex++;
			    }
			
		    	if (ptList[ptIndex].Label != minLabel){
				    ptList[ptIndex].Label = minLabel;
				    changeFlag++;
		    	}	 
				 ptIndex++;
           }
		}//clusterCheck
		  
		double computeDist (Point p1,xyCoord p2){
		  	double dist= sqrt((p1.xCoord-p2.xCoord)*(p1.xCoord-p2.xCoord)+(p1.yCoord-p2.yCoord)*(p1.yCoord-p2.yCoord));
			return dist;
		}//computeDist
		  
		void printPtList (ostream & out){
		  	 for(int a=0;a<numPts;a++){
		  	 	out<<ptList[a].xCoord<<","<<ptList[a].yCoord<<","<<ptList[a].Label<<endl; 
		  	 }
		}//printPtList
	};//KMean

	class image{
		public:
		int numRows,numCols;
		int**imgAry;
		image(int a,int b){		
	    	numRows= a; 
	        numCols= b;
			imgAry = new int*[numRows];
		}
		 
		void mapPoint2Image(int numPts,Point*ptList){
		   
			for (int i = 0; i< numRows; i++){
				imgAry[i] = new int[numCols];
			}
			for(int i=0;i<numRows;i++){
				for(int j=0;j<numCols;j++)
					imgAry[i][j]=0;
			}
			for(int j=0;j<numPts;j++){		
				imgAry[ptList[j].xCoord][ptList[j].yCoord] = ptList[j].Label;
		    }
		}
		
		void PrettyPrintImgAry(int numRow,int numCol,ostream& myFile){	
		    	for(int i=0;i<numRow;i++){
			    	for(int j=0;j<numCol;j++){
				    	if (imgAry[i][j] > 0){
			               myFile<<imgAry[i][j];
			            }  
                       else myFile<<" ";
				   }
				myFile<<endl;
		       }              	
		}
};//image

int main(int argc, char* argv[]){	
    if(argc<4){
    	cout<<"You need 3 files."<<endl;
    	return 1;
	}
    ifstream inFile;	
	inFile.open(argv[1]); 
	int numRow,numCol,numPts;
	inFile>>numRow>>numCol>>numPts; 
				            							
	ofstream pointList;
	pointList.open(argv[2]);
	    
	ofstream imageArray;
	imageArray.open(argv[3]);
	
   	int kCluster;
	cout<<"Please enter a number:";
	cin>>kCluster;	

    Kmean b(kCluster,numPts); 
    b.loadPtList(inFile);
    b.assignLable();
    b.printPtList(pointList);
    
    image c(numRow,numCol);
    c.mapPoint2Image(numPts,b.ptList);
    c.PrettyPrintImgAry(numRow,numCol,imageArray);               
    b.changeFlag=0;
    b.computeCentroid(kCluster);
    b.clusterCheck(kCluster);
    
    while(b.changeFlag>0){	
          c.mapPoint2Image(numPts,b.ptList);
          c.PrettyPrintImgAry(numRow,numCol,imageArray);               
          b.changeFlag=0;
          b.computeCentroid(kCluster);
          b.clusterCheck(kCluster);
    }
    
    inFile.close();  
	pointList.close();
	imageArray.close();   
    return 0;
}

