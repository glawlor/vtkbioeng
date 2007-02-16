#include "vtkProperty.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkSphereSource.h"
#include "vtkMatrix4x4.h"
#include "vtkCollisionDetectionFilter.h"
#include "vtkInteractorStyleJoystickCamera.h"
#include "vtkCleanPolyData.h"
#include "vtkStripper.h"
#include "vtkClipPolyData.h"
#include "vtkSelectPolyData.h"
#include "vtkCellArray.h"
#include "vtkAppendPolyData.h"
#include "vtkSmartPointer.h"

int main()
{
   vtkSphereSource *sphere0 = vtkSphereSource::New();
   sphere0->SetPhiResolution(3);
   sphere0->SetThetaResolution(3);
   sphere0->SetCenter(-0.0, 0, 0);

   vtkSphereSource *sphere1 = vtkSphereSource::New();
   sphere1->SetPhiResolution(30);
   sphere1->SetThetaResolution(30);
   sphere1->SetRadius(0.3);

   vtkMatrix4x4 *matrix0 = vtkMatrix4x4::New();
   vtkMatrix4x4 *matrix1 = vtkMatrix4x4::New();

   vtkCollisionDetectionFilter *collide = vtkCollisionDetectionFilter::New();
   //collide->DebugOn();
   collide->SetInputConnection(0, sphere0->GetOutputPort());
   collide->SetMatrix(0, matrix0);
   collide->SetInputConnection(1, sphere1->GetOutputPort());
   collide->SetMatrix(1, matrix1);
   collide->SetBoxTolerance(0.0);
   collide->SetCellTolerance(0.0);
   collide->SetNumberOfCellsPerBucket(2);
   collide->SetCollisionModeToAllContacts();
   collide->GenerateScalarsOn();

   vtkCleanPolyData *clean = vtkCleanPolyData::New();
     clean->SetInputConnection(collide->GetContactsOutputPort());

   vtkStripper *stripper = vtkStripper::New();
     stripper->SetInputConnection(clean->GetOutputPort());
     stripper->Update();

   vtkSelectPolyData *select = vtkSelectPolyData::New();
     select->SetInputConnection(sphere1->GetOutputPort());
     select->GenerateSelectionScalarsOn();
     select->SetSelectionModeToSmallestRegion();

   vtkClipPolyData *clip = vtkClipPolyData::New();
     clip->SetInputConnection(select->GetOutputPort());
     clip->InsideOutOn();

   vtkAppendPolyData *append = vtkAppendPolyData::New();
   vtkSmartPointer<vtkPolyData> clipped;

   vtkCellArray *lines = stripper->GetOutput()->GetLines();
   vtkPoints *allPoints = stripper->GetOutput()->GetPoints();
   vtkIdType numPts, *pts;
   vtkPoints *points = vtkPoints::New();

   for (lines->InitTraversal(); lines->GetNextCell(numPts, pts); )
     {
     points->Reset();
     // don't close the loop by inserting the last point...
     // vtkSelectPolyData does it for you (and crashes if you do it)
     for (int i =0; i<numPts-1; i++)
       {
       points->InsertNextPoint(allPoints->GetPoint(pts[i]));
       }
     select->SetLoop(points);
     select->Modified();
     clip->Update();
     clipped = vtkSmartPointer<vtkPolyData>::New();
     clipped->DeepCopy(clip->GetOutput());
     append->AddInput(clipped);
     }

   vtkPolyDataMapper *mapper1 = vtkPolyDataMapper::New();
   mapper1->SetInputConnection(collide->GetOutputPort(0));
   vtkActor *actor1 = vtkActor::New();
   actor1->SetMapper(mapper1);
   (actor1->GetProperty())->BackfaceCullingOn();

   vtkPolyDataMapper *mapper2 = vtkPolyDataMapper::New();
   mapper2->SetInputConnection(append->GetOutputPort());
   vtkActor *actor2 = vtkActor::New();
   actor2->SetMapper(mapper2);
   actor2->SetScale(2);

   vtkPolyDataMapper *mapper3 = vtkPolyDataMapper::New();
   mapper3->SetInputConnection(stripper->GetOutputPort());
   vtkActor *actor3 = vtkActor::New();
   actor3->SetMapper(mapper3);
   (actor3->GetProperty())->SetColor(0,0,0);
   (actor3->GetProperty())->SetLineWidth(3.0);


   vtkRenderer *ren = vtkRenderer::New();
   ren->AddActor(actor1);
   ren->AddActor(actor2);
   ren->AddActor(actor3);
   ren->SetBackground(1.0,1.0,1.0);

   vtkRenderWindow *renWin = vtkRenderWindow::New();
   renWin->AddRenderer(ren);

   vtkInteractorStyleJoystickCamera *istyle = vtkInteractorStyleJoystickCamera::New();
   vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
   iren->SetRenderWindow(renWin);
   iren->SetInteractorStyle(istyle);


   renWin->Render();
   iren->Start(); 
 
   sphere0->Delete();
   sphere1->Delete();
   matrix0->Delete();
   matrix1->Delete();
   collide->Delete();
   clean->Delete();
   stripper->Delete();
   select->Delete();
   clip->Delete();
   mapper1->Delete();
   mapper2->Delete();
   mapper3->Delete();
   actor1->Delete();
   actor2->Delete();
   actor3->Delete();
   ren->Delete();
   renWin->Delete();
   istyle->Delete();
}