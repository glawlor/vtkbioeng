/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCollisionDetection.h
  Language:  C++
  RCS:   $Id$

  Copyright (c) 2003-2004 Goodwin Lawlor
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.

=========================================================================*/
// .NAME vtkCollisionDetection - performs collision determination between two polyhedral surfaces
// .SECTION Description
// vtkCollisionDetection performs collision determination between two polyhedral surfaces using
// two instances of vtkOBBTree. Set the polydata inputs, the tolerance and transforms or matrices

// .SECTION Caveats
// Currently only triangles are processed. Use vtkTriangleFilter to
// convert any strips or polygons to triangles.

// .SECTION Thanks
// Goodwin Lawlor, University College Dublin.

// .SECTION See Also
// vtkTriangleFilter

#ifndef __vtkCollisionDetection_h
#define __vtkCollisionDetection_h

#include "vtkProcessObject.h"
#include "vtkBioengConfigure.h" // Include configuration header.

#include "vtkIdList.h"
class vtkOBBTree;
class vtkPolyData;
class vtkPoints;
class vtkMatrix4x4;


#include "vtkTransform.h"

// Collision modes
#define VTK_ALL_CONTACTS 0
#define VTK_FIRST_CONTACT 1

class VTK_BIOENG_EXPORT vtkCollisionDetection : public vtkProcessObject
{
public:
  // Description:
  // Constructs with initial values.
  static vtkCollisionDetection *New();

  vtkTypeRevisionMacro(vtkCollisionDetection, vtkProcessObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  int IntersectPolygonWithPolygon(int npts, double *pts, double bounds[6],
                                            int npts2, double *pts2, 
                                            double bounds2[6], double tol2,
                                            double x[3]);
  
  // Description:
  // Set and Get the input vtk polydata models
    void SetInput(int i, vtkPolyData *model);
    vtkPolyData *GetInput(int i);
    
      
  // Description:
  // Get a list of the contacting cells
      vtkIdList *GetContactCells(int i) {this->Update(); return this->ContactCells[i];}

  // Description:
  //  Get a list of points where the contacting cells intersect
      vtkGetObjectMacro(ContactPoints, vtkPoints);


  // Description:
  // Specify the transform object used to transform models.
  void SetTransform(int i, vtkAbstractTransform *transform);
  vtkAbstractTransform *GetTransform(int i) {this->Update(); return this->Transform[i];}
  
  // Description:
  // Specify the matrix object used to transform models. Used if transform are NULL
  void SetMatrix(int i, vtkMatrix4x4 *matrix);
  vtkMatrix4x4 *GetMatrix(int i) {return this->Matrix[i];}
  
  //Description:
  // Set and Get the obb tolerance (absolute value, in world coords). Default is 0.001
  vtkSetMacro(BoxTolerance, float);
  vtkGetMacro(BoxTolerance, float);

  //Description:
  // Set and Get the cell tolerance (squared value). Default is 0.0
  vtkSetMacro(CellTolerance, double);
  vtkGetMacro(CellTolerance, double);
  
  //Description:
  // Get the number of contacts
  int GetNumberOfContacts() {this->Update(); return this->ContactCells[0]->GetNumberOfIds();}
  
  //Description:
  // Get the number of box tests
  int GetNumberOfBoxTests() {this->Update(); return this->NumberOfBoxTests;}
    
  //Description:
  // Set and Get the number of cells in each OBB. Default is 2
  vtkSetMacro(NumberOfCellsPerBucket, int);
  vtkGetMacro(NumberOfCellsPerBucket, int);
  
  // Description:
  // Set the collision mode if you want to find all the contacts or quickly
  // find the first contact
  vtkSetClampMacro(CollisionMode,int,VTK_ALL_CONTACTS,VTK_FIRST_CONTACT);
  vtkGetMacro(CollisionMode,int);
  void SetCollisionModeToAllContacts() {this->SetCollisionMode(VTK_ALL_CONTACTS);};
  void SetCollisionModeToFirstContact() {this->SetCollisionMode(VTK_FIRST_CONTACT);};
  char *GetCollisionModeAsString();

  // Description:
  // Return the MTime also considering the transform.
  unsigned long GetMTime();
  
  void Update();

protected:
  vtkCollisionDetection();
  ~vtkCollisionDetection();
  
  vtkOBBTree *treeA;
  vtkOBBTree *treeB;
  
  vtkMatrix4x4 *tmpMatrix;

  vtkAbstractTransform *Transform[2];
  vtkMatrix4x4 *Matrix[2];

  vtkIdType NumberOfInputs;
  
  vtkIdList *ContactCells[2];
  
  vtkPoints *ContactPoints;
  
  int NumberOfBoxTests;
  
  int NumberOfCellsPerBucket;
  
  int GeneratePolydataOutput;
  
  float BoxTolerance;
  float CellTolerance;
  
  int CollisionMode;
                                            
  void Execute();
  vtkTimeStamp ExecuteTime;

private:  

  vtkCollisionDetection(const vtkCollisionDetection&);  // Not implemented.
  void operator=(const vtkCollisionDetection&);  // Not implemented.
};

#endif

inline char *vtkCollisionDetection::GetCollisionModeAsString(void)
{
  if ( this->CollisionMode == VTK_ALL_CONTACTS )
    {
    return (char *)"AllContacts";
    }
  else
    {
    return (char *)"FirstContact";
    }
}

