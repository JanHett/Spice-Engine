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

*Note on in/outputs: optional In/Outputs are wrapped in [ square brackets ]. In/outputs wrapped in [[double square brackets]] are optional and hidden until they are relevant. Unspecified obligatory in/outputs are automatically deferred to the parent recipe.*

### Data I/O

#### Asset

Reads or writes a source file.

As of yet unimplemented.

##### Inputs

- Path: `string` - the path to read from or write to
- [ Source: `any` ] - the data to write to the path

##### Outputs

- [[ Data: `any` ]] - the data read from the path (if it exists)

#### Colour

As of yet unimplemented.

#### Text

As of yet unimplemented.

### Image Processing

#### Blur

As of yet unimplemented.

#### Curves

As of yet unimplemented.

#### Convolution

As of yet unimplemented.

#### Bandpass

As of yet unimplemented.

#### Crop

Sets the dimensions of the input matrix.

As of yet unimplemented.

##### Inputs

- Source: `matrix`
- Width: `int`
- Height: `int` 
- Offset: `array<int, 2>` - where to move the crop window relative to the source matrix

##### Outputs

- [ Data: `matrix` ]

### Transformation

#### Transform

As of yet unimplemented.

## Feature Goals

- Portability: given that it has a UI for it, it should run on any of the five major operating systems
- Quality: output should equal or surpass the quality achievable with high-end editing software
- Speed: it should crunch through editing 150MP images in real time on a mid-range MacBook Pro or equivalent machines (i.e. maxed out 13" version, basic 15" version from two generations back)
- Generality: it should work with as simple or as complex a processing pipeline as the user wants
- Extensibility: new filters and transformations should be addable without much effort (and preferably using a well-known plugin API like OpenFX) and run at native speed
- Pluggability: it should be trivially extensible to be usable with any plugin interface (e.g. PS, OpenFX)

