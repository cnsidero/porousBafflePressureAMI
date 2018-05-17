/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2014 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::porousBafflePressureAMIFvPatchField

Group
    grpCoupledBoundaryConditions

Description
    This boundary condition provides a jump condition, across non-conformal
    cyclic path-pairs, employing an arbitraryMeshInterface (AMI).

    The porous baffle introduces a pressure jump defined by:

        \f[
            \Delta p = -(D \mu U + 0.5 I \rho |U|^2 )L
        \f]

    where

    \vartable
        p      | pressure [Pa]
        \rho   | density [kg/m3]
        \mu    | laminar viscosity [Pa s]
        I      | inertial coefficient
        D      | Darcy coefficient
        L      | porous media length in the flow direction
    \endvartable


    \heading Patch usage

    \table
        Property     | Description             | Required    | Default value
        patchType    | underlying patch type should be \c cyclic| yes |
        D            | Darcy coefficient       | yes         |
        I            | inertial coefficient    | yes         |
        L            | porous media length in the flow direction | yes |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            porousBafflePressureAMI;
        patchType       cyclic;
        jump            uniform 0;
        I               1000000;
        D               0.001;
        L               0.1;
        value           uniform 0;
    }
    \endverbatim

Note
     The underlying \c patchType should be set to \c cyclic

SourceFiles
    porousBafflePressureAMIFvPatchField.C

\*---------------------------------------------------------------------------*/

#ifndef porousBafflePressureAMIFvPatchField_H
#define porousBafflePressureAMIFvPatchField_H

#include "fixedJumpAMIFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                Class porousBafflePressureAMIFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class porousBafflePressureAMIFvPatchField
:
    public fixedJumpAMIFvPatchField<Type>
{
    // Private data

        //- Darcy pressure loss coefficient
        scalar D_;

        //- Inertia pressure lost coefficient
        scalar I_;

        //- Porous media length
        scalar length_;


public:

    //- Runtime type information
    TypeName("porousBafflePressureAMI");


    // Constructors

        //- Construct from patch and internal field
        porousBafflePressureAMIFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        porousBafflePressureAMIFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given porousBafflePressureAMIFvPatchField
        // onto a new patch
        porousBafflePressureAMIFvPatchField
        (
            const porousBafflePressureAMIFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        porousBafflePressureAMIFvPatchField
        (
            const porousBafflePressureAMIFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new porousBafflePressureAMIFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        porousBafflePressureAMIFvPatchField
        (
            const porousBafflePressureAMIFvPatchField<Type>&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchField<Type> > clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return tmp<fvPatchField<Type> >
            (
                new porousBafflePressureAMIFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        //- Write
        virtual void write(Ostream&) const;
};


//- Specialisation of the jump-condition for the pressure
template<>
void porousBafflePressureAMIFvPatchField<scalar>::updateCoeffs();


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::porousBafflePressureAMIFvPatchField<Type>::porousBafflePressureAMIFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedJumpAMIFvPatchField<Type>(p, iF),
    D_(0),
    I_(0),
    length_(0)
{}


template<class Type>
CML::porousBafflePressureAMIFvPatchField<Type>::porousBafflePressureAMIFvPatchField
(
    const porousBafflePressureAMIFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedJumpAMIFvPatchField<Type>(ptf, p, iF, mapper),
    D_(ptf.D_),
    I_(ptf.I_),
    length_(ptf.length_)
{}


template<class Type>
CML::porousBafflePressureAMIFvPatchField<Type>::porousBafflePressureAMIFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    fixedJumpAMIFvPatchField<Type>(p, iF),
    D_(readScalar(dict.lookup("D"))),
    I_(readScalar(dict.lookup("I"))),
    length_(readScalar(dict.lookup("length")))
{
    fvPatchField<Type>::operator=
    (
        Field<Type>("value", dict, p.size())
    );
}


template<class Type>
CML::porousBafflePressureAMIFvPatchField<Type>::porousBafflePressureAMIFvPatchField
(
    const porousBafflePressureAMIFvPatchField<Type>& ptf
)
:
    fixedJumpAMIFvPatchField<Type>(ptf),
    D_(ptf.D_),
    I_(ptf.I_),
    length_(ptf.length_)
{}


template<class Type>
CML::porousBafflePressureAMIFvPatchField<Type>::porousBafflePressureAMIFvPatchField
(
    const porousBafflePressureAMIFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedJumpAMIFvPatchField<Type>(ptf, iF),
    D_(ptf.D_),
    I_(ptf.I_),
    length_(ptf.length_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


template<class Type>
void CML::porousBafflePressureAMIFvPatchField<Type>::write(Ostream& os) const
{
    fixedJumpAMIFvPatchField<Type>::write(os);
    os.writeKeyword("D") << D_ << token::END_STATEMENT << nl;
    os.writeKeyword("I") << I_ << token::END_STATEMENT << nl;
    os.writeKeyword("length") << length_ << token::END_STATEMENT << nl;
}


#endif

// ************************************************************************* //
