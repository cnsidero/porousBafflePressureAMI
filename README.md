## Description
This boundary condition provides a jump condition, using the cyclicAMI condition as a base. The porous baffle introduces a pressure jump defined by:

&Delta; p = -(D &mu; U + 0.5 I &rho; |U|^2 )L

where

|Variable | Description |
|:------|:--------------|
|p      | pressure [Pa] |
|&rho;   | density [kg/m3] |
|&mu;   | laminar viscosity [Pa s] |
|I      | inertial coefficient |
|D      | Darcy coefficient |
|L      | porous media thickness in the flow direction [m] |


### Patch usage

|Property   | Description            | Required | Default value 
|:----------|:-----------------------|:---------|:--------------
|patchType  | should be cyclicAMI    | yes      | 
|D          | Darcy coefficient      | yes      | 0 
|I          | inertial coefficient   | yes      | 0 
|length     | porous media thickness | yes      | 0 

<br>
Example of the boundary condition specification:

```
myPatch
{
    type            porousBafflePressureAMI;
    patchType       cyclicAMI;
    jump            uniform 0;
    I               10000;
    D               0.001;
    length          0.1;
    value           uniform 0;
}
```
## Compilation
To compile, provide the path to the Caelus `site_scons` directory. E.g.

```
scons --site-dir="/home/user/Caelus/Caelus-8.04/site_scons" install
```

If successful, the library will be installed in `CAELUS_USER_LIBBIN`. E.g.

```
/home/user/Caelus/user-8.04/platforms/linux64g++DPOpt/lib
```

Finally, add the following to the `controlDict` to make the BC available to the solver.

```
libs ( "libporousBafflePressureAMIFvPatchField.so" );
```

## License

This software is compatible with <a href="https://bitbucket.org/appliedccm/caelus-contributors">Caelus</a>.

It is released under the GPL: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

## Contact
- Chris Sideroff: c.sideroff@appliedccm.ca

## COPYRIGHT Applied CCM 2018

Tested version: Caelus 8.04


