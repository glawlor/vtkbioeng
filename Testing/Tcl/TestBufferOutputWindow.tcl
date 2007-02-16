package require vtk
package require vtkbioeng

wm withdraw .

vtkBufferOutputWindow buffer
    buffer SetInstance buffer
    buffer AddObserver ErrorEvent UpdateText
    buffer AddObserver WarningEvent UpdateText
    # UserEvent is observed since there's no DebugEvent
    buffer AddObserver UserEvent UpdateText
    
vtkTextProperty txtProp
    txtProp AntiAliasingOff
    txtProp SetFontSize 16
    
vtkTextActor txtActor
    txtActor SetTextProperty txtProp

vtkConeSource cone
    cone DebugOn

vtkPolyDataMapper cMapper
    cMapper SetInput [cone GetOutput]
vtkActor cActor
    cActor SetMapper cMapper
    

   
vtkRenderer ren
    ren AddActor cActor
    ren AddActor2D txtActor

vtkRenderWindow renWin
    renWin AddRenderer ren
vtkRenderWindowInteractor iren
    iren SetRenderWindow renWin
    iren AddObserver UserEvent {console show}
    [iren GetPicker] DebugOn
    iren Initialize
   
    
        
proc UpdateText {} {

    # Format the text depending on its type
    switch [buffer GetLastOutputTextTypeAsString] {
        "Error Text" {
            txtProp SetColor 1 0 0
            txtProp BoldOn
            txtProp ItalicOff
        }
        "Warning Text" {
            txtProp SetColor 0 0 1
            txtProp BoldOff
            txtProp ItalicOn
        }
        "Generic Warning Text" {
            txtProp SetColor 0 0 1
            txtProp BoldOff
            txtProp ItalicOn
        }
        "Debug Text" {
            txtProp SetColor 1 1 1
            txtProp BoldOff
            txtProp ItalicOff
            }
    }
    
    # Strip away the output text preceding the final colon
    set txt [buffer GetLastOutputText]
    # Get the index of the final colon
    set index [expr [string last : $txt] +1]
    # Get the string from the colon onwards
    set txt [string range $txt $index end]
    txtActor SetInput $txt
    renWin Render

  
}

# Provoke some debug text
renWin Render
cone SetHeight 3
after 2000
ren ResetCamera
renWin Render





        