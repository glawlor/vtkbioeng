/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRemoveCellsFilter.h
  Language:  C++
  Version:   $Id$

  Copyright (c) Goodwin Lawlor 2003-2004
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkRemoveCellsFilter- actually remove a cell from a vtkPolyData object
// .SECTION Description
// vtkRemoveCellsFilter removes cells one at a time. vtkCellPicker could be used to get the CellId of the cell to
// be removed. Then call the RemoveCell method with the CellId. The UndoRemoveCells method restores the 
// PolyData to it original state. Alternatively, you can mark cells with MarkCell and remove them all in one go
// with RemoveMarkedCells. This is much more efficient for large datasets since the filter only has to execute
// once.

// .SECTION Caveats
// Trying to remove the last remaining cell in a dataset will restore all the removed cells!
// Any filter between this filter and the vtkPolyDataMapper should not alter the cell order


// .SECTION Thanks
// Goodwin Lawlor, University College Dublin


#ifndef __vtkRemoveCellsFilter_h
#define __vtkRemoveCellsFilter_h

#include "vtkCellsFilter.h"
// #include "vtkIdList.h"
// #include "vtkCharArray.h"
// #include "vtkPolyData.h"
#include "vtkBioengConfigure.h" // Include configuration header.

class vtkLookupTable;

class VTK_BIOENG_EXPORT vtkRemoveCellsFilter : public vtkCellsFilter
{
public:
  // Description:
  // Constructs with initial values of zero.
  static vtkRemoveCellsFilter *New();

  vtkTypeRevisionMacro(vtkRemoveCellsFilter, vtkCellsFilter);
  void PrintSelf(ostream& os, vtkIndent indent);
  
  // Description:
  // Remove a cell from the input polydata.  cellid is a cell id from the input to the filter
  // A recursive trick remaps the new cellids (from the output) to the old cellids (at the input) 
  // so that the cellid returned by vtkCellPicker is the correct one to use.
 void RemoveCell(vtkIdType cellid);
 
  // Description:
  // Remove the cells marked by MarkCell(cellid)
 void RemoveMarkedCells();
 
//   // Description:
//   // Remove everything but the cells marked by MarkCell(cellid) or ToggleCells(cellid)
//   void RemoveUnmarkedCells();
 
 // Description:
 // Undo all the remove operations 
 void UndoRemoveCells(){this->Initialize(); this->Modified();}
 

 // Desciption:
 // Get the number of cells that have been removed. GetNumberOfDeletedCells should be depreciated.
 int GetNumberOfDeletedCells(){return this->GetPolyDataInput(0)->GetNumberOfCells() - vtkPolyData::SafeDownCast(this->GetOutput())->GetNumberOfCells();}
 int GetNumberOfRemovedCells(){return this->GetPolyDataInput(0)->GetNumberOfCells() - vtkPolyData::SafeDownCast(this->GetOutput())->GetNumberOfCells();} 



protected:
  vtkRemoveCellsFilter(){};
  ~vtkRemoveCellsFilter(){};
  

private:
  vtkRemoveCellsFilter(const vtkRemoveCellsFilter&);  // Not implemented.
  void operator=(const vtkRemoveCellsFilter&);  // Not implemented.
};

#endif


