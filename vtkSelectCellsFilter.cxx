/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkSelectCellsFilter.cxx
  Language:  C++
  Version:   $Id$

  Copyright (c) 2004 Goodwin Lawlor
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE. 

=========================================================================*/
#include "vtkSelectCellsFilter.h"
#include "vtkIdList.h"
#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkSelectCellsFilter, "$Revision$");
vtkStandardNewMacro(vtkSelectCellsFilter);

void vtkSelectCellsFilter::RemoveUnmarkedCells()
{

  vtkIdType numIds = this->CellIdList->GetNumberOfIds();
  vtkIdType id;
  
  for (vtkIdType i = 0; i < numIds; i++)
    {
    id = this->CellIdList->GetId(i);
    if (this->MarkedCellIdList->IsId(id) == -1)
      {
      this->CellIdList->DeleteId(id);
      }
    }
    
  this->InitializeScalars();
  this->Modified();
}

void vtkSelectCellsFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Number of cells removed: "
     << this->GetNumberOfRemovedCells()
     << "\n";
}
