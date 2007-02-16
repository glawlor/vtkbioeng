/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkSelectCellsFilter.h
  Language:  C++
  Version:   $Id$

  Copyright (c) Goodwin Lawlor 2004
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkSelectCellsFilter- actually remove a cell from a vtkPolyData object
// .SECTION Description
// vtkSelectCellsFilter allows you to mark cells (be they polygons, lines etc.) that will be exclusively 
// passed to the output of the filter by removing the unmarked cells.

// .SECTION Thanks
// Goodwin Lawlor, University College Dublin


#ifndef __vtkSelectCellsFilter_h
#define __vtkSelectCellsFilter_h

#include "vtkCellsFilter.h"
#include "vtkBioengConfigure.h" // Include configuration header.

class vtkLookupTable;

class VTK_BIOENG_EXPORT vtkSelectCellsFilter : public vtkCellsFilter
{
public:
  // Description:
  // Constructs with initial values of zero.
  static vtkSelectCellsFilter *New();

  vtkTypeRevisionMacro(vtkSelectCellsFilter, vtkCellsFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Remove everything but the cells marked by MarkCell(cellid) or ToggleCells(cellid)
  void RemoveUnmarkedCells();
 
 // Description:
 // Undo all the remove operations 
 void UndoRemoveCells(){this->Initialize(); this->Modified();}
 
 // Desciption:
 // Get the number of cells that have been removed. GetNumberOfDeletedCells should be depreciated.
 int GetNumberOfDeletedCells(){return this->GetPolyDataInput(0)->GetNumberOfCells() - vtkPolyData::SafeDownCast(this->GetOutput())->GetNumberOfCells();}
 int GetNumberOfRemovedCells(){return this->GetPolyDataInput(0)->GetNumberOfCells() - vtkPolyData::SafeDownCast(this->GetOutput())->GetNumberOfCells();} 



protected:
  vtkSelectCellsFilter(){};
  ~vtkSelectCellsFilter(){};
  

private:
  vtkSelectCellsFilter(const vtkSelectCellsFilter&);  // Not implemented.
  void operator=(const vtkSelectCellsFilter&);  // Not implemented.
};

#endif


