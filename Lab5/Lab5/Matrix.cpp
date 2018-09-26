#include "Matrix.h"
#include <iostream>

#define nullptr NULL

using namespace std;

Matrix::Matrix(int rows,int columns) {
    if (rows>0 && columns>0) {
        this->rows=rows;
        this->columns=columns;
        offset=new int[rows];
        size=new int[rows];
        data=new double *[rows];
        for (int row=0; row<rows; row++) {
            size[row]=0;
            offset[row]=0;
            data[row]=nullptr;
        }
    } else {
        this->rows=0;
        
        this->columns=0;
        offset=nullptr;
        size=nullptr;
        data=nullptr;
    }
}

void Matrix::clear() {
    for (int row=0; row<rows; row++) delete[] data[row];
    delete[] offset; offset=nullptr;
    delete[] size; size=nullptr;
    delete[] data; data=nullptr;
    rows=columns=0;
}

int Matrix::memoryUsed() const {
    int bytes=0;
    bytes+=rows*sizeof(int);           //Memory used by dynamic array of sizes
    bytes+=rows*sizeof(int);           //Memory used by dynamic array of offsets
    bytes+=rows*sizeof(double *);      //Memory used by dynamic array of pointers
    for (int row=0; row<rows; row++)
        bytes+=size[row]*sizeof(double); //Memory used to store data within row
    return bytes;
}


double Matrix::get(int row,int column) const {
    if (row<0 || row>=rows) return 0.0;
    column-=offset[row];
    if (column<0 || column>=size[row]) return 0.0;
    return data[row][column];
}

void Matrix::set(int row, int col, double input) {
    // Check value of input and conditions relating to column number
    // and row offset/size to determine how to update Matrix data
    
    int currentOffset = offset[row];
    int oldSize; // size of row, for special conditions and determining new values later
    int lastInd = offset[row] + size[row] - 1;
    double *dataHolder; // will contain data values in the row
    double dummyData = 0.0;
    bool inOffsetRegion = 0;
    
    if (size[row] == 0) {
        dataHolder = &dummyData;
        oldSize = 0;
    } else {
        dataHolder = data[row];
        oldSize = size[row];
    }
    
    if (input == 0.0 && oldSize == 1 && col == currentOffset) {
        // If the only value in the row is set to 0
        size[row] = 0;
        offset[row] = 0;
        data[row] = nullptr;
    } else {
        if (input == 0.0) {
            if (col == currentOffset) {
                // If the first nonzero value in the row is set to 0
                data[row][0] = input;
                dataHolder = data[row];
                for (int i = 0; i < oldSize; i++) {
                    if (data[row][i] != 0) {
                        // Determining new size and offset values after change
                        size[row] = oldSize - i;
                        offset[row] = currentOffset + i;
                        break;
                    }
                }
            } else if (col == lastInd) {
                // If 0 is put into last index of the row
                data[row][col-offset[row]] = input;
                dataHolder = data[row];
                for (int i = (oldSize - 1); i > -1; i--) {
                    if (data[row][i] != 0) {
                        size[row] = i + 1;
                        break;
                    }
                }
            }
            
            data[row] = new double[size[row]];
            for (int j = 0; j < size[row]; j++) {
                data[row][j] = 0;
            }
            
            for (int j = 0; j < size[row]; j++) {
                data[row][j] = *(dataHolder + j);
            }
            
        } else {
            if (col < offset[row]) {
                // If new data is being placed in the offset region
                inOffsetRegion = 1;
                offset[row] = col;
            } else if (col > offset[row] && size[row] == 0) {
                // If new data will be the only data in the row
                offset[row] = col;
            }
            
            if (size[row] == 0) { // updating data if the size is 0
                lastInd = col;
                size[row] = 1;
            } else {
                //update size based on what col is
                if (col == offset[row]) {
                    size[row] = lastInd - offset[row] + 1;
                    
                } else if (col > lastInd) {
                    size[row] = col - offset[row] + 1;
                }
            }
            
            // Create new data array to be filled in
            data[row] = new double[size[row]];
            for (int j = 0; j < size[row]; j++) {
                data[row][j] = 0;
            }
            
            for (int j = 0; j < oldSize; j++) {
                if (!inOffsetRegion) {
                    data[row][j] = *(dataHolder + j);
                } else {
                    int sub = currentOffset - offset[row];
                    data[row][j+sub] = *(dataHolder + j);
                }
            }
            
            // Placing input in correct index of data array based on col value
            if (col == offset[row]) {
                data[row][0] = input;
            } else if (col > offset[row] && col < ((offset[row] + size[row] - 1))) {
                data[row][col-offset[row]] = input;
            } else if (col > offset[row]) {
                data[row][col-offset[row]] = input;
            }
        }
    }
}

void Matrix::deepCopy(const Matrix &other) {
    this->rows = other.rows;
    this->columns = other.columns;
    
    size = new int[rows]; //Allocate new size array
    for (int i = 0; i < rows; i++) {
        size[i] = other.size[i];
    }
    
    offset = new int[rows]; //Allocate new offset array
    for (int i = 0; i < rows; i++) {
        offset[i] = other.offset[i];
    }
    
    data = new double *[rows]; //Allocate new data array
    for (int i = 0; i < rows; i++) {
        data[i] = new double[size[i]]; //Allocate subarray
        for (int j = 0; j < size[i]; j++) {
            data[i][j] = other.data[i][j];
        }
    }
}

Matrix Matrix::operator - () const {
    Matrix newMatrix = Matrix(*this);
    newMatrix = newMatrix * -1;
    return newMatrix;
}

Matrix Matrix::operator * (double scalar) const {
    Matrix newMatrix = Matrix(*this);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < size[i]; j++) {
            newMatrix.data[i][j] = scalar * data[i][j];
        }
    }
    
    return newMatrix;
}

Matrix Matrix::operator + (const Matrix & a) const {
    
    //Return an empty array if dimensions of operands are not compatible
    
    if (columns!=a.columns || rows!=a.rows) return Matrix(0,0);
    
    //Otherwise, set dimensions of answer to match those of operands
    
    Matrix ans(rows,columns);
    
    //Now perform addition row by row including dyanmic memory
    //allocation for each corresponding sub-row pointer in data[].
    //(Note: this code assumes that entries in size[] have been set
    //       to zero for all rows contaning no non-zero entries)
    
    int start1,start2,start; //Index of first non-zero items of current row
    //within both operand arrays and the answer array.
    int stop1,stop2,stop;    //Index just past last non-zero item of current
    //row within both the operand and answer arrays.
    int column,row;
    
    for (row=0; row<rows; row++) {
        
        //Skip current row if neither array contains non-zero row items
        
        if (size[row]==0 && a.size[row]==0) {
            ans.size[row]=0; ans.data[row]=nullptr;
            continue;
        }
        
        //Find beginning and end of sub-rows within the two operand arrays
        
        if (size[row]==0) {
            start2=a.offset[row];  stop2=start2+a.size[row];
            start1=stop1=start2;
        } else if (a.size[row]==0) {
            start1=  offset[row];  stop1=start1+  size[row];
            start2=stop2=start1;
        } else {
            start1=  offset[row];  stop1=start1+  size[row];
            start2=a.offset[row];  stop2=start2+a.size[row];
        }
        
        //Define ITEM_1/ITEM_2/ITEM_ANS (array items at current column/row)
        
#define ITEM_1       data[row][column-    offset[row]]  //left operand
#define ITEM_2     a.data[row][column-  a.offset[row]]  //right operand
#define ITEM_ANS ans.data[row][column-ans.offset[row]]  //result
        
        //Determine beginning and end of sub-row within the answer array
        
        if (start1==start2) {
            column=start1;
            while (column<stop1 && column<stop2 && ITEM_1+ITEM_2==0) column++;
            if (column>=stop1) while (column<stop2 && ITEM_2==0) column++;
            if (column>=stop2) while (column<stop1 && ITEM_1==0) column++;
            start1=start2=column;
        }
        
        if (stop1==stop2) {
            column=stop1-1;
            while (column>=start1 && column>=start2 && ITEM_1+ITEM_2==0) column--;
            if (column<start1) while (column>=start2 && ITEM_2==0) column--;
            if (column<start2) while (column>=start1 && ITEM_1==0) column--;
            stop1=stop2=column+1;
        }
        
        start= start1<=start2 ? start1 : start2;
        stop=   stop1>=stop2  ?  stop1 : stop2 ;
        
        //Allocate memory for data in sub-row of answer array
        
        ans.size[row]= stop-start;
        ans.offset[row]=start;
        
        if (ans.size[row]==0) {
            ans.offset[row]=0; ans.data[row]=nullptr;
            continue;
        } else {
            ans.data[row]=new double[ans.size[row]];
        }
        
        //Compute values in sub-row
        
        for (column=start; column<stop; column++) {
            ITEM_ANS=0;
            if (column>=start1 && column<stop1) ITEM_ANS+=ITEM_1;
            if (column>=start2 && column<stop2) ITEM_ANS+=ITEM_2;
        }
        
        //Undefine ITEM_1/ITEM_2/ITEM_ANS (not necessary but clean coding)
        
#undef ITEM_1
#undef ITEM_2
#undef ITEM_ANS
    }
    
    return ans;
}

std::ostream & operator << (std::ostream &os,const Matrix &m) {
    os << std::endl;
    os << "----- matrix represented -----" << std::endl;
    int row,col;
    for (row=0; row<m.rows; row++) {
        for (col=0; col<m.columns; col++) os << m.get(row,col) << '\t';
        os << std::endl;
    }
    os << "----- data values stored -----" << std::endl;
    for (row=0; row<m.rows; row++) {
        for (int k=0; k<m.offset[row]; k++)
            os << '~' << '\t';
        for (int k=0; k<m.size[row]; k++)
            os << m.data[row][k] << '\t';
        for (int k=m.offset[row]+m.size[row]; k<m.rows; k++)
            os << '~' << '\t';
        os << std::endl;
    }
    os << "---- internal object info ----" << std::endl;
    for (row=0; row<m.rows; row++) {
        os << "offset[" << row << "]=" << m.offset[row];
        os << "\tsize[" << row << "]=" << m.size[row] << std::endl;
    }
    os << "dynamic memory used=" << m.memoryUsed() << " bytes" << std::endl;
    return os;
}
