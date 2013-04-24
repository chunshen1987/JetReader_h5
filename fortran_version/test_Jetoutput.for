      Program main

      USE HDF5 ! This module contains all necessary modules
      Implicit none

      Integer, parameter :: XL = -130
      Integer, parameter :: XH = 130
      Integer, parameter :: YL = -130
      Integer, parameter :: YH = 130
      
      Integer, parameter :: LSX = 5
      Integer, parameter :: LSY = 6
      Integer, parameter :: LST = 2
      
      double precision :: DX = 0.1
      double precision :: DY = 0.1
      double precision :: Tau0 = 0.6
      double precision :: dTau = 0.02
      integer :: Frame_id
      Double precision, Dimension(XL:XH, YL:YH, 1:1):: Ed, Sd,
     &                                         P, Temp, Vx, Vy
      Double precision, Dimension(XL:XH, YL:YH, 1:1):: Pi00, Pi01,
     &            Pi02, Pi03, Pi11, Pi12, Pi13, Pi22, Pi23, Pi33
      Double precision, Dimension(XL:XH, YL:YH, 1:1):: BulkPi

      double precision :: ee, pp, ss, TT, vxx, vyy, deltau
      double precision :: jet_avglength, jet_avglength_in, 
     &                    jet_avglength_out
      
!      Ed = 10.4d0
!      Sd = 1.0d0
!      P = 0.0d0
!      Temp = 1.0d0
!      Vx = 1.0d0
!      Vy = 0.5d0
!      Pi00 = 0.0d0
!      Pi01 = 0.0d0
!      Pi02 = 0.0d0
!      Pi03 = 0.0d0
!      Pi11 = 0.0d0
!      Pi12 = 0.0d0
!      Pi13 = 0.0d0
!      Pi22 = 0.0d0
!      Pi23 = 0.0d0
!      Pi33 = 0.0d0
!      BulkPi = 0.0d0
!      Call setHydroFiles(XL, XH, DX, LSX, YL, YH, DY, LSY, Tau0, dTau, 
!     &                   LST)
!      
!      do Frame_id = 0, 10
!         Call writeHydroBlock(Frame_id, Ed, Sd, P, Temp, Vx, Vy,
!     &      Pi00, Pi01, Pi02, Pi03, Pi11, Pi12, Pi13, Pi22, Pi23, Pi33, 
!     &      BulkPi)
!      enddo

      Call readHydroFiles_initialEZ("JetData.h5")
      Call readHydroinfoBuffered_ideal(3.4d0, 2.5d0, -4.34d0, 
     &  ee,pp,ss,TT,vxx,vyy);
      write(*,*) TT
      call getJetDeltaTauMax(0.0d0,0.0d0,1.0d0,0.0d0, 0.3d0, 0.05d0
     &                       ,deltau)
      write(*,*) deltau
!      call getJetavgLength_shell(0.12d0, 10.0d0, jet_avglength, 
!     &   jet_avglength_in, jet_avglength_out)
!      write(*,*) jet_avglength, jet_avglength_in, jet_avglength_out

      stop
      end

