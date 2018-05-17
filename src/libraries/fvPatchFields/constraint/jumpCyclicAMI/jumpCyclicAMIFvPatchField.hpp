/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2012 OpenFOAM Foundation
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
    CML::jumpCyclicAMIFvPatchField

Group
    grpCoupledBoundaryConditions

Description
    This boundary condition provides a base class that enforces a cyclic
    condition with a specified 'jump' (or offset) between a pair of boundaries,
    whereby communication between the patches is performed using an arbitrary
    mesh interface (AMI) interpolation.

SeeAlso
    CML::cyclicAMIFvPatchField

SourceFiles
    jumpCyclicAMIFvPatchField.C

\*---------------------------------------------------------------------------*/

#ifndef jumpCyclicAMIFvPatchField_H
#define jumpCyclicAMIFvPatchField_H

#include "cyclicAMIFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                  Class jumpCyclicAMIFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class jumpCyclicAMIFvPatchField
:
    public cyclicAMIFvPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("jumpCyclicAMI");


    // Constructors

        //- Construct from patch and internal field
        jumpCyclicAMIFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        jumpCyclicAMIFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given jumpCyclicAMIFvPatchField onto a
        //  new patch
        jumpCyclicAMIFvPatchField
        (
            const jumpCyclicAMIFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        jumpCyclicAMIFvPatchField
        (
            const jumpCyclicAMIFvPatchField<Type>&
        );

        //- Construct as copy setting internal field reference
        jumpCyclicAMIFvPatchField
        (
            const jumpCyclicAMIFvPatchField<Type>&,
            const DimensionedField<Type, volMesh>&
        );


    // Member functions

        // Access

            //- Return the interface type
            virtual const word& interfaceFieldType() const
            {
                return cyclicAMIFvPatchField<Type>::type();
            }

            //- Return the "jump" across the patch as a "half" field
            virtual tmp<Field<Type> > jump() const = 0;


        // Evaluation functions

            //- Return neighbour coupled given internal cell data
            tmp<Field<Type> > patchNeighbourField() const;

            //- Update result field based on interface functionality
            virtual void updateInterfaceMatrix
            (
                scalarField& result,
                const scalarField& psiInternal,
                const scalarField& coeffs,
                const direction cmpt,
                const Pstream::commsTypes commsType
            ) const;

            //- Update result field based on interface functionality
            virtual void updateInterfaceMatrix
            (
                Field<Type>&,
                const Field<Type>&,
                const scalarField&,
                const Pstream::commsTypes commsType
            ) const;
};


//- Update result field based on interface functionality
template<>
void jumpCyclicAMIFvPatchField<scalar>::updateInterfaceMatrix
(
    scalarField& result,
    const scalarField& psiInternal,
    const scalarField& coeffs,
    const direction cmpt,
    const Pstream::commsTypes commsType
) const;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::jumpCyclicAMIFvPatchField<Type>::jumpCyclicAMIFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    cyclicAMIFvPatchField<Type>(p, iF)
{}


template<class Type>
CML::jumpCyclicAMIFvPatchField<Type>::jumpCyclicAMIFvPatchField
(
    const jumpCyclicAMIFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    cyclicAMIFvPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
CML::jumpCyclicAMIFvPatchField<Type>::jumpCyclicAMIFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    cyclicAMIFvPatchField<Type>(p, iF, dict)
{
    // Call this evaluation in derived classes
    //this->evaluate(Pstream::blocking);
}


template<class Type>
CML::jumpCyclicAMIFvPatchField<Type>::jumpCyclicAMIFvPatchField
(
    const jumpCyclicAMIFvPatchField<Type>& ptf
)
:
    cyclicAMIFvPatchField<Type>(ptf)
{}


template<class Type>
CML::jumpCyclicAMIFvPatchField<Type>::jumpCyclicAMIFvPatchField
(
    const jumpCyclicAMIFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    cyclicAMIFvPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::Field<Type> >
CML::jumpCyclicAMIFvPatchField<Type>::patchNeighbourField() const
{
    const Field<Type>& iField = this->internalField();
    const labelUList& nbrFaceCells =
        this->cyclicAMIPatch().cyclicAMIPatch().neighbPatch().faceCells();

    Field<Type> pnf(iField, nbrFaceCells);
    tmp<Field<Type> > tpnf;

    if (this->cyclicAMIPatch().applyLowWeightCorrection())
    {
        tpnf =
            this->cyclicAMIPatch().interpolate
            (
                pnf,
                this->patchInternalField()()
            );
    }
    else
    {
        tpnf = this->cyclicAMIPatch().interpolate(pnf);
    }

    if (this->doTransform())
    {
        tpnf = transform(this->forwardT(), tpnf);
    }

    tmp<Field<Type> > tjf = jump();
    if (!this->cyclicAMIPatch().owner())
    {
        tjf = -tjf;
    }

    return tpnf - tjf;
}


template<class Type>
void CML::jumpCyclicAMIFvPatchField<Type>::updateInterfaceMatrix
(
    scalarField& result,
    const scalarField& psiInternal,
    const scalarField& coeffs,
    const direction cmpt,
    const Pstream::commsTypes
) const
{
    notImplemented
    (
        "void CML::jumpCyclicAMIFvPatchField<Type>::updateInterfaceMatrix"
        "("
            "scalarField&, "
            "const scalarField&, "
            "const scalarField& coeffs,"
            "const direction, "
            "const Pstream::commsTypes"
        ") const"
    );
}


template<class Type>
void CML::jumpCyclicAMIFvPatchField<Type>::updateInterfaceMatrix
(
    Field<Type>& result,
    const Field<Type>& psiInternal,
    const scalarField& coeffs,
    const Pstream::commsTypes
) const
{
    const labelUList& nbrFaceCells =
        this->cyclicAMIPatch().cyclicAMIPatch().neighbPatch().faceCells();

    Field<Type> pnf(psiInternal, nbrFaceCells);

    if (this->cyclicAMIPatch().applyLowWeightCorrection())
    {
        pnf =
            this->cyclicAMIPatch().interpolate
            (
                pnf,
                this->patchInternalField()()
            );

    }
    else
    {
        pnf = this->cyclicAMIPatch().interpolate(pnf);
    }

    // only apply jump to original field
    if (&psiInternal == &this->internalField())
    {
        Field<Type> jf(this->jump());
        if (!this->cyclicAMIPatch().owner())
        {
            jf *= -1.0;
        }

        pnf -= jf;
    }

    // Transform according to the transformation tensors
    this->transformCoupleField(pnf);

    // Multiply the field by coefficients and add into the result
    const labelUList& faceCells = this->cyclicAMIPatch().faceCells();
    forAll(faceCells, elemI)
    {
        result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
    }
}


#endif

// ************************************************************************* //
