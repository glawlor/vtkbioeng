/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCellsFilter.cxx
  Language:  C++
  Version:   $Id$

  Copyright (c) 2004 Goodwin Lawlor
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE. 

=========================================================================*/
#include "vtkCellsFilter.h"
#include "vtkIdList.h"
#include "vtkObjectFactory.h"
#include "vtkPolyData.h"
#include "vtkCellArray.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkCellArray.h"
#include "vtkCharArray.h"
#include "vtkLookupTable.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"

vtkCxxRevisionMacro(vtkCellsFilter, "$Revision$");


// Constructs with initial  values.
vtkCellsFilter::vtkCellsFilter()
{
  this->CellIdList= vtkIdList::New();
  this->MarkedCellIdList = vtkIdList::New();
  this->IsInitialized = 0;
  this->IsScalarsInitialized = 0;
  this->Scalars = vtkCharArray::New();
  this->Scalars->SetNumberOfComponents(1);
  this->Scalars->SetName("MarkedCells");
  this->MarkedColor[0] = 1.0;
  this->MarkedColor[0] = 0.0;
  this->MarkedColor[0] = 0.0;
  this->UnmarkedColor[0] = 1.0;
  this->UnmarkedColor[0] = 1.0;
  this->UnmarkedColor[0] = 1.0;
  this->MarkedOpacity = 1.0;
  this->LUT = vtkLookupTable::New();
  this->LUT->SetNumberOfColors(2);
  this->LUT->SetTableValue(0, 1.0, 1.0, 1.0, 1.0);
  this->LUT->SetTableValue(1, 1.0, 0.0, 0.0, 1.0);
  this->LUT->Build();
  this->Scalars->SetLookupTable(this->LUT);
}

// Destroy any allocated memory.
vtkCellsFilter::~vtkCellsFilter()
{
  this->CellIdList->Delete();
  this->MarkedCellIdList->Delete();
  this->Scalars->Delete();
  this->LUT->Delete();
}

void vtkCellsFilter::SetMarkedColor(double r, double g, double b)
{
  this->LUT->SetTableValue(1, r, g, b, this->MarkedOpacity);
  this->MarkedColor[0] = r;
  this->MarkedColor[0] = g;
  this->MarkedColor[0] = b;
  this->LUT->Build();
  
}

void vtkCellsFilter::SetUnmarkedColor(double r, double g, double b)
{
  this->LUT->SetTableValue(0, r, g, b, 1.0);
  this->UnmarkedColor[0] = r;
  this->UnmarkedColor[0] = g;
  this->UnmarkedColor[0] = b;
  this->LUT->Build();
  
}

void vtkCellsFilter::SetMarkedOpacity(double opacity)
{
  double rgba[4];
  this->LUT->GetTableValue(1, rgba);
  this->LUT->SetTableValue(1, rgba[0], rgba[1], rgba[2], opacity);
  this->MarkedOpacity = opacity;
  this->LUT->Build();
}


// Description:
// Perform cell removal
int vtkCellsFilter::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  if (!this->IsInitialized)
    {
    this->Initialize();
    }
    
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and ouptut
  vtkPolyData *input = vtkPolyData::SafeDownCast(
    inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData *output = vtkPolyData::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));
  
  // num of cells that will be in the output
  vtkIdType numCells = this->CellIdList->GetNumberOfIds();
  // dont know how many points... cant be more than the input
  vtkIdType numPts = input->GetNumberOfPoints();
  
  vtkPointData *outPD = output->GetPointData();
    outPD->CopyAllocate(input->GetPointData(), numPts);
  vtkCellData *outCD = output->GetCellData();
    outCD->CopyAllocate(input->GetCellData(),numCells);    
    output->Allocate(input,numCells);
    
    // Copy unremoved cells to the output... 
    output->CopyCells(input, this->CellIdList);

  return 1;
  
}

void vtkCellsFilter::Initialize()
{
  // This updates the pipeline upstream so that we can get the number
  // of cells... horrific vtk filter design, but what else will work? I could
  // document to explicitly call ->Update() on previous filter. It wont
  // matter if the user lets the vtk pipeline execute once first and heads
  // into the event loop like the filter was intended to do originally.
  
  // This also gets around the problem when the user is in the event
  // loop but some filter has been modified upstream, changing the data.
  this->GetPolyDataInput(0)->Update();
  
  vtkIdType numCells = this->GetPolyDataInput(0)->GetNumberOfCells();
  this->CellIdList->SetNumberOfIds(numCells);
  
  for (vtkIdType i=0; i < numCells; i++)
    {
    this->CellIdList->SetId(i,i);
    }
    
  this->IsInitialized = 1;
}

void vtkCellsFilter::InitializeScalars()
{
  // CellIdList will have been initialized first
  vtkIdType numCells = this->CellIdList->GetNumberOfIds();
  this->Scalars->SetNumberOfValues(numCells);
  

  
  for (vtkIdType i=0; i < numCells; i++)
    {
    this->Scalars->SetValue(i,0);
    }
  
  this->MarkedCellIdList->Reset();
  this->GetOutput()->GetCellData()->SetScalars(this->Scalars);
  this->IsScalarsInitialized = 1;
}

void vtkCellsFilter::ToggleCell(vtkIdType cellid_at_output)
{
  vtkIdType cellid_at_input;
  
  // If the input to the filter has changed since the last execution
  // we need
  
  if (!this->IsInitialized)
    {
    this->Initialize();
    }
    
  // only initialize scalars if MarkCell or ToggleCells is used
  if (!this->IsScalarsInitialized)
    {
    this->InitializeScalars();
    }

  cellid_at_input = this->CellIdList->GetId(cellid_at_output);
  

  
  if (this->MarkedCellIdList->IsId(cellid_at_input) == -1)
    {
    this->MarkedCellIdList->InsertNextId(cellid_at_input); 
    this->Scalars->SetValue(cellid_at_output, 1);
    }
  else
    {
    this->MarkedCellIdList->DeleteId(cellid_at_input); 
    this->Scalars->SetValue(cellid_at_output, 0);
    }
    
  //do this here to avoid re-executing this filter...
  this->GetOutput()->GetCellData()->SetScalars(this->Scalars);
  this->Scalars->Modified();
    
}

void vtkCellsFilter::UnmarkCell(vtkIdType cellid_at_output)
{
  vtkIdType cellid_at_input;
  
  // check if anything has been marked first  
  if (!this->IsScalarsInitialized)
    {
    return;
    }

  // see comment below
  cellid_at_input = this->CellIdList->GetId(cellid_at_output);
  
  
  this->MarkedCellIdList->DeleteId(cellid_at_input); 
  this->Scalars->SetValue(cellid_at_output, 0);
  
  //do this here to avoid re-executing this filter...
  this->Scalars->Modified();
    
}

void vtkCellsFilter::MarkCell(vtkIdType cellid_at_output)
{
  vtkIdType cellid_at_input;
  
  if (!this->IsInitialized)
    {
    this->Initialize();
    }
    
  // only initialize scalars if MarkCell is used
  if (!this->IsScalarsInitialized)
    {
    this->InitializeScalars();
    }

  // see comment below
  cellid_at_input = this->CellIdList->GetId(cellid_at_output);
  
  
  this->MarkedCellIdList->InsertNextId(cellid_at_input); 
  this->Scalars->SetValue(cellid_at_output, 1);
  
  //do this here to avoid re-executing this filter...
  this->GetOutput()->GetCellData()->SetScalars(this->Scalars);
  this->Scalars->Modified();
    
}


void vtkCellsFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

}
