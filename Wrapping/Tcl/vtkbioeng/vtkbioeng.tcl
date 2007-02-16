catch {
    package require vtk
}


#
# Here you should pick the name of one your local classes


if {[info commands vtkRemoveCellsFilter] != "" ||
    [::vtk::load_component vtkBioengTCL] == ""} {
    package provide vtkbioeng 4.5
}
