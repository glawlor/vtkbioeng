/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkBufferOutputWindow.h
  Language:  C++
  RCS:   $Id$

  Copyright (c) 2004 Goodwin Lawlor

=========================================================================*/
// .NAME vtkBufferOutputWindow - 
// .SECTION Description
// Writes debug/warning/error output to a buffer (a class variable) which can be accessed from a 
// wrapped language (tcl etc.). The class invokes ErrorEvent, WarningEvent, GenericWarningEvent and
// UserEvent (ie DebugEvent doesn't exist) which can be observed and a callback triggered to get the 
// output text.
//
// To use this class, instantiate it and then call SetInstance(this).


#ifndef __vtkBufferOutputWindow_h
#define __vtkBufferOutputWindow_h

#include "vtkOutputWindow.h"
#include "vtkBioengConfigure.h" // Include configuration header.


class VTK_BIOENG_EXPORT vtkBufferOutputWindow : public vtkOutputWindow
{
public:
  vtkTypeRevisionMacro(vtkBufferOutputWindow, vtkOutputWindow);

  static vtkBufferOutputWindow* New();
  
//BTX
  enum TextType
  {
      TEXT,
      WARNING_TEXT,
      ERROR_TEXT,
      GENERIC_WARNING_TEXT,
      DEBUG_TEXT
  };
//ETX

  // Description:
  // Get the last text type: 0 = TEXT; 1 = WARNING_TEXT; 2 = ERROR_TEXT; 
  // 3 = GENERIC_WARNING_TEXT; 4 = DEBUG_TEXT
  vtkGetMacro(LastOutputTextType, int);

  // Description:
  // the output window api
  virtual void DisplayText(const char*);
  virtual void DisplayErrorText(const char*);
  virtual void DisplayWarningText(const char*);
  virtual void DisplayGenericWarningText(const char*);
  virtual void DisplayDebugText(const char*);

  
  // Description:
  // Get the last message
  vtkGetStringMacro(LastOutputText);
  
  // Description:
  // Get the last message type... error, warning etc.
  char *GetLastOutputTextTypeAsString();


 protected:
  vtkBufferOutputWindow(); 
  virtual ~vtkBufferOutputWindow(); 
  
  char *LastOutputText;
  int LastOutputTextType;

  private:
  vtkBufferOutputWindow(const vtkBufferOutputWindow&);  // Not implemented.
  void operator=(const vtkBufferOutputWindow&);  // Not implemented.
};

inline char *vtkBufferOutputWindow::GetLastOutputTextTypeAsString(void)
{
  if ( this->LastOutputTextType == TEXT )
    {
    return (char *)"Text";
    }
  else if ( this->LastOutputTextType == ERROR_TEXT )
    {
    return (char *)"Error Text";
    }
  else if ( this->LastOutputTextType == WARNING_TEXT )
    {
    return (char *)"Warning Text";
    }
  else if ( this->LastOutputTextType == GENERIC_WARNING_TEXT )
    {
    return (char *)"Generic Warning Text";
    }
  else
    {
    return (char *)"Debug Text";
    }
}

#endif
