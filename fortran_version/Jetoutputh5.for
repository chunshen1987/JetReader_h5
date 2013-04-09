!***********************************************************************
      Subroutine setHydroFiles(XL_in, XH_in, DX_in, LSX_in, 
     &                         YL_in, YH_in, DY_in, LSY_in, 
     &                         Tau0_in, dTau_in, LST_in)
      
      Use HDF5
      Implicit none

      CHARACTER(LEN=10) :: H5hydroFilename = "JetData.h5" ! File name
      CHARACTER(LEN=8) :: groupname = "/Event" ! Group name
      Common /dataFile/ H5hydroFilename, groupname

      Integer :: XL_in, XH_in, YL_in, YH_in
      Double precision :: DX_in, DY_in, Tau0_in, dTau_in
      Integer :: LSX_in, LSY_in, LST_in

      Integer :: XL, XH, YL, YH
      Double precision :: DX, DY, Tau0, dTau
      Integer :: LSX, LSY, LST, LST_cur
      Integer:: XShift, YShift
      Common /hydroInfo/ XL, XH, DX, YL, YH, DY, Tau0, dTau
      Common /sparse/ XShift, LSX, YShift, LSY, LST, LST_cur
      
      Integer :: OutputViscousFlag = 1 ! Flag for whether to output shear stress tensor
      Common /OutputCtl/ OutputViscousFlag

      INTEGER(HID_T) :: file_id       ! File identifier
      INTEGER(HID_T) :: group_id      ! Group identifier

      INTEGER     ::   error ! Error flag

      XL = XL_in 
      XH = XH_in
      DX = DX_in
      YL = YL_in
      YH = YH_in
      DY = DY_in
      Tau0 = Tau0_in
      dTau = dTau_in

      LSX = LSX_in
      LSY = LSY_in
      LST = LST_in
      LST_cur = 0
      XShift = Abs(Mod(XL, LSX_in))
      YShift = Abs(Mod(YL, LSY_in))

      ! Initialize FORTRAN interface.
      CALL h5open_f(error)

      ! Create a new file using default properties.
      CALL h5fcreate_f(H5hydroFilename, H5F_ACC_TRUNC_F, file_id, error)

      ! Create group "Event" using absolute name.
      CALL h5gcreate_f(file_id, groupname, group_id, error)

      ! Write Attribute for group "Event"
      Call writeGroupattribute(group_id)

      ! Close the groups.
      CALL h5gclose_f(group_id, error)

      ! Terminate access to the file.
      CALL h5fclose_f(file_id, error)

      ! Close FORTRAN interface.
      CALL h5close_f(error)
      end
!-----------------------------------------------------------------------

!***********************************************************************
      Subroutine writeGroupattribute(group_id)
      Use HDF5
      Implicit none

      Integer :: XL, XH, YL, YH
      Double precision :: DX, DY, Tau0, dTau
      Integer :: LSX, LSY, LST, LST_cur
      Integer:: XShift, YShift

      Common /hydroInfo/ XL, XH, DX, YL, YH, DY, Tau0, dTau
      Common /sparse/ XShift, LSX, YShift, LSY, LST, LST_cur

      INTEGER(HID_T) :: group_id      ! Group identifier

      Call addGroupattributeInt(group_id, "XL",(XL+XShift)/LSX)
      Call addGroupattributeInt(group_id, "XH",(XH-XShift)/LSX)
      Call addGroupattributeInt(group_id, "YL",(YL+YShift)/LSY)
      Call addGroupattributeInt(group_id, "YH",(YH-YShift)/LSY)
      Call addGroupattributeDouble(group_id, "DX", DX*LSX)
      Call addGroupattributeDouble(group_id, "DY", DY*LSY)
      Call addGroupattributeDouble(group_id, "Tau0", Tau0)
      Call addGroupattributeDouble(group_id, "dTau", dTau*LST)

      end
!-----------------------------------------------------------------------

!***********************************************************************
      Subroutine addGroupattributeInt(group_id, aname, avalue)
      Use HDF5
      Implicit none

      CHARACTER(LEN=*) :: aname       ! Attribute name
      Integer :: avalue      ! Attribute value
      INTEGER(HID_T) :: group_id      ! Group identifier

      INTEGER(HID_T) :: attr_id       ! Attribute identifier
      INTEGER(HID_T) :: aspace_id     ! Attribute Dataspace identifier
      Integer(HID_T) :: atype_id
      
      INTEGER(HSIZE_T), DIMENSION(1) :: adims = (/1/) ! Attribute dimension
      INTEGER     ::   arank = 1                      ! Attribure rank
      
      INTEGER     ::   error ! Error flag
     
      ! Create scalar data space for the attribute.
      CALL h5screate_simple_f(arank, adims, aspace_id, error)

      ! Create dataset attribute.
      CALL h5acreate_f(group_id, aname, H5T_NATIVE_INTEGER, aspace_id,
     &                  attr_id, error)
     
      ! Write the attribute data.
      CALL h5awrite_f(attr_id, H5T_NATIVE_INTEGER, avalue, adims, error)
     
      ! Close the attribute.
      CALL h5aclose_f(attr_id, error)

      ! Terminate access to the data space.
      CALL h5sclose_f(aspace_id, error)

      end
!-----------------------------------------------------------------------

!***********************************************************************
      Subroutine addGroupattributeDouble(group_id, aname, avalue)
      Use HDF5
      Implicit none

      CHARACTER(LEN=*) :: aname       ! Attribute name
      double precision :: avalue      ! Attribute value
      INTEGER(HID_T) :: group_id      ! Group identifier

      INTEGER(HID_T) :: attr_id       ! Attribute identifier
      INTEGER(HID_T) :: aspace_id     ! Attribute Dataspace identifier
      
      INTEGER(HSIZE_T), DIMENSION(1) :: adims = (/1/) ! Attribute dimension
      INTEGER     ::   arank = 1                      ! Attribure rank
      
      INTEGER     ::   error ! Error flag
     
      ! Create scalar data space for the attribute.
      CALL h5screate_simple_f(arank, adims, aspace_id, error)

      ! Create dataset attribute.
      CALL h5acreate_f(group_id, aname, H5T_NATIVE_DOUBLE, aspace_id,
     &                  attr_id, error)
     
      ! Write the attribute data.
      CALL h5awrite_f(attr_id, H5T_NATIVE_DOUBLE, avalue, adims, error)
     
      ! Close the attribute.
      CALL h5aclose_f(attr_id, error)

      ! Terminate access to the data space.
      CALL h5sclose_f(aspace_id, error)

      end
!-----------------------------------------------------------------------

!***********************************************************************
      Subroutine writeHydroBlock(Time_id, Ed, Sd, P, Temp, Vx, Vy, 
     &   Pi00, Pi01, Pi02, Pi03, Pi11, Pi12, Pi13, Pi22, Pi23, Pi33, 
     &   BulkPi)

      Use HDF5
      Implicit none

      CHARACTER(LEN=10) :: H5hydroFilename ! File name
      CHARACTER(LEN=8) :: groupname        ! Group name
      CHARACTER(LEN=10) :: frameName       ! Group frame name
      Character(Len=4) :: frame_id_string
      Common /dataFile/ H5hydroFilename, groupname

      Integer :: XL, XH, YL, YH
      Double precision :: DX, DY, Tau0, dTau
      Integer :: LSX, LSY, LST, LST_cur
      Integer:: XShift, YShift
      Common /hydroInfo/ XL, XH, DX, YL, YH, DY, Tau0, dTau
      Common /sparse/ XShift, LSX, YShift, LSY, LST, LST_cur

      Integer :: OutputViscousFlag
      Common /OutputCtl/ OutputViscousFlag

      INTEGER(HID_T) :: file_id       ! File identifier
      INTEGER(HID_T) :: group_id      ! Group identifier
      INTEGER(HID_T) :: group_frame_id
      INTEGER(HID_T) :: dataset_id    ! Dataset identifier
      INTEGER(HID_T) :: dataspace_id  ! Data space identifier
      
      INTEGER     ::   error ! Error flag

      Integer :: Time_id, Frame_id
      Double precision, Dimension(XL:XH, YL:YH, 1:1) :: Ed, Sd, 
     &                  P, Temp, Vx, Vy
      Double precision, Dimension(XL:XH, YL:YH, 1:1):: Pi00, Pi01,
     &            Pi02, Pi03, Pi11, Pi12, Pi13, Pi22, Pi23, Pi33
      Double precision, Dimension(XL:XH, YL:YH, 1:1):: BulkPi
      
      INTEGER(HSIZE_T), DIMENSION(2) :: dims
      
      if(LST_cur /= 0) then  !no writing action
         LST_cur = LST_cur - 1
         return
      else   ! write to file and reset recurse idx
         LST_cur = LST - 1
      endif

      dims(1) = (XH - XL - 2*XShift)/LSX + 1
      dims(2) = (YH - YL - 2*YShift)/LSY + 1
      
      Frame_id = floor(DBLE(Time_id)/LST)
      write(unit=frame_id_string, fmt='(I4.4)') Frame_id
      frameName = "Frame_" // frame_id_string 

      ! Initialize FORTRAN interface.
      CALL h5open_f(error)

      ! Open an existing file.
      CALL h5fopen_f (H5hydroFilename, H5F_ACC_RDWR_F, file_id, error)
      
      ! Open an existing group in the specified file.
      CALL h5gopen_f(file_id, groupname, group_id, error)

      ! Create group "Frame_i" in group "Event" using relative name.
      CALL h5gcreate_f(group_id, frameName, group_frame_id, error)
      
      Call addGroupattributeDouble(group_frame_id, "Time", 
     &                             Tau0 + dTau*LST*Frame_id)

      ! Dump data into h5 file
      Call CSH5dumpBlockdata(group_frame_id, dims, "e", Ed)
      Call CSH5dumpBlockdata(group_frame_id, dims, "s", Sd)
      Call CSH5dumpBlockdata(group_frame_id, dims, "P", P)
      Call CSH5dumpBlockdata(group_frame_id, dims, "Temp", Temp)
      Call CSH5dumpBlockdata(group_frame_id, dims, "Vx", Vx)
      Call CSH5dumpBlockdata(group_frame_id, dims, "Vy", Vy)
      if(OutputViscousFlag .eq. 1) then
         Call CSH5dumpBlockdata(group_frame_id, dims, "Pi00", Pi00)
         Call CSH5dumpBlockdata(group_frame_id, dims, "Pi01", Pi01)
         Call CSH5dumpBlockdata(group_frame_id, dims, "Pi02", Pi02)
         Call CSH5dumpBlockdata(group_frame_id, dims, "Pi03", Pi03)
         Call CSH5dumpBlockdata(group_frame_id, dims, "Pi11", Pi11)
         Call CSH5dumpBlockdata(group_frame_id, dims, "Pi12", Pi12)
         Call CSH5dumpBlockdata(group_frame_id, dims, "Pi13", Pi13)
         Call CSH5dumpBlockdata(group_frame_id, dims, "Pi22", Pi22)
         Call CSH5dumpBlockdata(group_frame_id, dims, "Pi23", Pi23)
         Call CSH5dumpBlockdata(group_frame_id, dims, "Pi33", Pi33)
         Call CSH5dumpBlockdata(group_frame_id, dims, "BulkPi", BulkPi)
      endif

      ! Close the group.
      CALL h5gclose_f(group_frame_id, error)
      CALL h5gclose_f(group_id, error)

      ! Close the file.
      CALL h5fclose_f(file_id, error)

      ! Close FORTRAN interface.
      CALL h5close_f(error)

      end
!-----------------------------------------------------------------------

!***********************************************************************
      subroutine CSH5dumpBlockdata(group_id, dims, DatasetName, Dataset)
      Use HDF5
      Implicit none
      
      Character(Len=*) :: DatasetName
      Integer :: XL, XH, YL, YH
      Double precision :: DX, DY, Tau0, dTau
      Integer :: LSX, LSY, LST, LST_cur
      Integer:: XShift, YShift
      Common /hydroInfo/ XL, XH, DX, YL, YH, DY, Tau0, dTau
      Common /sparse/ XShift, LSX, YShift, LSY, LST, LST_cur

      INTEGER(HID_T) :: group_id      ! Group identifier
      INTEGER(HID_T) :: dataset_id    ! Dataset identifier
      INTEGER(HID_T) :: dataspace_id  ! Data space identifier

      INTEGER(HSIZE_T), DIMENSION(2) :: dims ! Datasets dimensions
      Double precision, Dimension(XL:XH, YL:YH, 1:1) :: Dataset

      Integer :: error
      Integer :: rank = 2

      ! Create the data space for the first dataset.
      CALL h5screate_simple_f(rank, dims, dataspace_id, error)

      ! Create the dataset in group "Frame_i" with default properties.
      CALL h5dcreate_f(group_id, DatasetName, H5T_NATIVE_DOUBLE, 
     &                 dataspace_id, dataset_id, error)
      ! Write the first dataset.
      CALL h5dwrite_f(dataset_id, H5T_NATIVE_DOUBLE, 
     &   Dataset(XL+XShift:XH-XShift:LSX, YL+YShift:YH-YShift:LSY, 1:1),
     &   dims, error)

      ! Close the dataspace for the dataset.
      CALL h5sclose_f(dataspace_id, error)

      ! Close the dataset.
      CALL h5dclose_f(dataset_id, error)

      end
!-----------------------------------------------------------------------
