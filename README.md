# RasterTile

The RasterTile project is a core project in the Terrafly System to process and provide raster imagery (including satellite images and aerial photographs).

This project is created for two purposes. First is to convert satellite/aerial imageries from various sources/formats/projections to a uniform format and projection, and store them in a special designed local imagery database. The second purpose is to provide a .NET based web service that can read data from the internal database, and serve these data in the form of web API.


## Project Structure

The project is comprised of four sub-projects: a core library written with C++, a CLR project that converts C++ library to C# interface, a Loader written with C#, and a reader written with ASP.NET

* TileImagerySystem
    * The core library written with C++. It provides all the important functionalities that is needed.

* TileImagerySystem_CLR
    * A CLR project that converts C++ library to a C# library.

* TileImageryLoader
    * A GUI based imagery loader, which makes it easier to convert imagery from various formats to locally stored format.

* TileImageryReader
    * A .NET based web service that can read data from the database and serve them as web API.


## How to build the project

Please refer to doc/build_and_deploy.md


## User's Guide

To be completed.
