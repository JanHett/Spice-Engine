# Spice Engine
An image processor utilising node-based, composable recipes.

Part of the documentation provided here can be seen as a roadmap for future features. Eventually the more implementation- and usage-related parts will be specified in a doxygen document.

## Data Structures

### Recipe

A node graph. If at least one in- or output is exposed, the recipe can be referenced from another (called the parent recipe) as a node.

### Node

A filter, transform or other modification to be applied to zero or more data structures, resulting in zero or more data structures. Data flows in and out through a potentially variable number of in- and outputs.

### Input

Potentially optional data source for a node. Specifying values for both optional and obligatory nodes can be deferred to the parent recipe. Deferring value specification does not imply the absence of a value (e.g. one might be provided for preview purposes or as a default).

### Output

Product of a node. Can be fed to an input of compatible type or exposed as the product of the recipe (thus exposing an output in the recipe's node in the parent recipe).

### Matrix\<T>

Abstract, two dimensional set of values.

### Image: Matrix<Pixel<4>>

A specification of the `matrix` class exposing additional image specific operations.

### Primitives

float, int, string etc...

## Node Types

*Note on in/outputs:* optional inputs are wrapped in [ square brackets ]. Unspecified obligatory inputs are automatically deferred to the parent recipe.
In/outputs wrapped in [[double square brackets]] are optional and hidden until they are relevant.

Currently unimplemented node types are set in *italic*. The planned release version is noted after the pipe "|" (some features might be missing upon release).

### Data I/O

#### *Asset | v 0.1.0*

Reads or writes a source file.

As of yet unimplemented.

##### Inputs

- Path: `string` - the path to read from or write to
- [ Source: `any` ] - the data to write to the path

##### Outputs

- [[ Data: `any` ]] - the data read from the path (if it exists)

#### *Recipe | v 0.1.0*

Loads another recipe into the current one.

As of yet unimplemented.

##### Inputs

- Recipe Inputs: `any[]` - all exposed inputs of the child recipe

##### Outputs

- Recipe Outputs: `any[]` - all exposed outputs of the child recipe

#### *Plane | v 0.1.0*

Generates a solid colour or gradient on an infinite canvas

As of yet unimplemented.

##### Inputs

- Colours: `map<point, rgba>` - colours of the plane - if the map contains exactly one element, the plane has a solid colour

##### Outputs
- Data: `image`

#### *Text | v 0.2.0*

Typesets a string in a particular font.

As of yet unimplemented.

##### Inputs

- String: `string`
- Font: `string` // TODO: check if a simple search string is enough for fontconfig
- Font size: `uint16_t`

##### Outputs
- Data: `image`

### Image Processing

#### *Blend | v 0.1.0*

Combines a stack of images into one.

As of yet unimplemented.

##### Inputs

- Images: `pair<image, matrix<float>(3,3)>[]` - the images to be blended along with their respective transforms relative to the origin of the blend

##### Outputs

- Data: `image`

#### *Blur | v 0.1.0*

As of yet unimplemented.

##### Inputs

##### Outputs
- Data: `image`

#### *Curves | v 0.2.0*

As of yet unimplemented.

##### Inputs

##### Outputs

- Data: `image`

#### *Convolution | v 0.2.0*

As of yet unimplemented.

##### Inputs

##### Outputs

- Data: `image`

#### *Bandpass | v 0.3.0*

As of yet unimplemented.

##### Inputs

##### Outputs

- Data: `image`

#### *Crop | v 0.1.0*

Sets the dimensions of the input matrix.

As of yet unimplemented.

##### Inputs

- Source: `image`
- Transform: `matrix<float>(3,3)` - size, position and rotation of the crop window relative to the source matrix

##### Outputs

- Data: `image`

### Transformation

#### *Transform | v 0.1.0*

A widget to enter transformation data. Multiple transform nodes can be chained to compound their transformations.

As of yet unimplemented.

##### Inputs

- Transform: `matrix<float>(3,3)` - position, scale and rotation

##### Outputs

- Transform: `matrix<float>(3,3)` - position, scale and rotation

## Feature Goals

- Portability: given that it has a UI for it, it should run on any of the five major operating systems
- Quality: output should equal or surpass the quality achievable with high-end editing software
- Speed: it should crunch through editing 150MP images in real time on a mid-range MacBook Pro or equivalent machines (i.e. maxed out 13" version, basic 15" version from two generations back)
- Generality: it should work with as simple or as complex a processing pipeline as the user wants
- Extensibility: new filters and transformations should be addable without much effort (and preferably using a well-known plugin API like OpenFX) and run at native speed
- Pluggability: it should be trivially extensible to be usable with any plugin interface (e.g. PS, OpenFX)

