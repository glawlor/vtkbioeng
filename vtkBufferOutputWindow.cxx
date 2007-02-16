/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkBufferOutputWindow.cxx
  Language:  C++
  RCS:   $Id$

  Copyright (c) 2003 Goodwin Lawlor

=========================================================================*/
#include "vtkBufferOutputWindow.h"
#include "vtkObjectFactory.h"
#include "vtkCommand.h"

vtkCxxRevisionMacro(vtkBufferOutputWindow, "$Revision$");
vtkStandardNewMacro(vtkBufferOutputWindow);

vtkBufferOutputWindow::vtkBufferOutputWindow()
{
  this->LastOutputText  =  NULL;
  this->LastOutputTextType = NULL;
}

vtkBufferOutputWindow::~vtkBufferOutputWindow()
{
  delete [] this->LastOutputText;
  this->LastOutputText = NULL;
  this->LastOutputTextType = NULL;
}


void vtkBufferOutputWindow::DisplayText(const char* text)
{
  this->LastOutputTextType = TEXT;
  if (this->LastOutputText) 
    {
    delete [] this->LastOutputText; 
    }
  this->LastOutputText = new char[strlen(text)+1]; 
  strcpy(this->LastOutputText, text);
  
}

void vtkBufferOutputWindow::DisplayErrorText(const char* text)
{
  this->LastOutputTextType = ERROR_TEXT;
  if (this->LastOutputText) 
    {
    delete [] this->LastOutputText; 
    }
  this->LastOutputText = new char[strlen(text)+1]; 
  strcpy(this->LastOutputText, text);
  
  if (this->HasObserver(vtkCommand::ErrorEvent) )
    {
    this->InvokeEvent(vtkCommand::ErrorEvent);
    }
}

void vtkBufferOutputWindow::DisplayWarningText(const char* text)
{
  this->LastOutputTextType = WARNING_TEXT;
  if (this->LastOutputText) 
    {
    delete [] this->LastOutputText; 
    }
  this->LastOutputText = new char[strlen(text)+1]; 
  strcpy(this->LastOutputText, text);
  
  if (this->HasObserver(vtkCommand::WarningEvent) )
    {
    this->InvokeEvent(vtkCommand::WarningEvent);
    }
}

void vtkBufferOutputWindow::DisplayGenericWarningText(const char* text)
{
  this->LastOutputTextType = GENERIC_WARNING_TEXT;
  if (this->LastOutputText) 
    {
    delete [] this->LastOutputText; 
    }
  this->LastOutputText = new char[strlen(text)+1]; 
  strcpy(this->LastOutputText, text);
  
  if (this->HasObserver(vtkCommand::WarningEvent) )
    {
    this->InvokeEvent(vtkCommand::WarningEvent);
    }
}

void vtkBufferOutputWindow::DisplayDebugText(const char* text)
{
  this->LastOutputTextType = DEBUG_TEXT;
  if (this->LastOutputText) 
    {
    delete [] this->LastOutputText; 
    }
  this->LastOutputText = new char[strlen(text)+1]; 
  strcpy(this->LastOutputText, text);

  // this is a "DebugEvent"
  if (this->HasObserver(vtkCommand::UserEvent) )
    {
    this->InvokeEvent(vtkCommand::UserEvent);
    }

}
