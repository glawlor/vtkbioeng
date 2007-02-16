# A tcl script to test vtkRemoveCellsFilter
# Press "p" to remove cells
# Pres "u" to restore removed cells
package require vtk
package require vtkinteraction
package require vtkbioeng

wm withdraw .

vtkSpherePuzzle puzzle

vtkStripper stripper
	stripper SetInput [puzzle GetOutput]

vtkRemoveCellsFilter remove
    remove SetInput [stripper GetOutput]
    remove SetUnmarkedColor 0.8 0.8 0.9
    remove SetMarkedColor 1 0 0
    #remove SetMarkedOpacity 0
    
vtkPolyDataMapper mapper
    mapper SetInput [remove GetOutput]
    mapper SetScalarModeToUseCellData
    
vtkActor actor
    actor SetMapper mapper
    [actor GetProperty] BackfaceCullingOn
    [actor GetProperty] SetColor 0.8 0.8 0.9
    
vtkCellPicker cpick
    cpick AddObserver EndPickEvent ToggleCell
    cpick SetTolerance 0.001
    
vtkRenderer ren
    ren AddActor actor
    ren SetBackground 1 1 1
vtkRenderWindow renwin
    renwin AddRenderer ren
vtkRenderWindowInteractor iren
    iren SetRenderWindow renwin
    iren AddObserver UserEvent {remove RemoveMarkedCells; renwin Render}
    iren SetPicker cpick
    iren Initialize
    
proc ToggleCell {} {
    set id [cpick GetCellId]
    puts "Picked cell $id\n"
    puts "Number of cells deleted: [remove GetNumberOfDeletedCells]"
    remove ToggleCell $id
    renwin Render
}

proc MarkCell {} {
    set id [cpick GetCellId]
    puts "Picked cell $id\n"
    puts "Number of cells deleted: [remove GetNumberOfDeletedCells]"
    remove MarkCell $id
    renwin Render
}


proc RemoveCell {} {
    set id [cpick GetCellId]
    puts "Picked cell $id\n"
    puts "Number of cells deleted: [remove GetNumberOfDeletedCells]"
    remove RemoveCell $id
    renwin Render
}

    



