/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRemoveCellsFilter.cxx
  Language:  C++
  Version:   $Id$

  Copyright (c) 2003-2004 Goodwin Lawlor
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE. 

=========================================================================*/
#include "vtkRemoveCellsFilter.h"
#include "vtkIdList.h"
#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkRemoveCellsFilter, "$Revision$");
vtkStandardNewMacro(vtkRemoveCellsFilter);


void vtkRemoveCellsFilter::RemoveMarkedCells()
{
  vtkIdType numIds = this->MarkedCellIdList->GetNumberOfIds();
  
  for (vtkIdType i = 0; i < numIds; i++)
    {
    this->CellIdList->DeleteId(this->MarkedCellIdList->GetId(i));
    }
    
  this->InitializeScalars();
  this->Modified();
}


void vtkRemoveCellsFilter::RemoveCell(vtkIdType cellid_at_output)
{
  vtkIdType cellid_at_input;
  
  if (!this->IsInitialized)
    {
    this->Initialize();
    }
  
  // Because vtkIdList reorders its id list in the same manner that vtkCellArray
  // reorders its cell list, CellIdList can be used to find out what cellid a cell has
  // at the filter input... as cells are deleted from the middle of the cell array, 
  // ids are reorder so they're monotonic increasing (1,2,3,4,...)
  cellid_at_input = this->CellIdList->GetId(cellid_at_output);
  
  this->CellIdList->DeleteId(cellid_at_input); 
  this->Modified();
}

void vtkRemoveCellsFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Number of cells removed: "
     << this->GetNumberOfRemovedCells()
     << "\n";

}
