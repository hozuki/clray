# CL-Ray

The C99 adaptation of Peter Shirley's [Ray Tracing in One Weekend](http://www.realtimerendering.com/raytracing/Ray%20Tracing%20in%20a%20Weekend.pdf) ([code ref](https://github.com/petershirley/raytracinginoneweekend)).

It is used as a performance reference for other implementations (e.g. in C#). It may be a foundation for some future experiments.

In addition to the features in Shirley's code, this adaptation also includes:

- uniformed material representation
- an iterative renderer
- cross-platform support

For a comparison basis, currently it runs at 60M to 70M samples/s (single-threaded, i7-4712MQ).

## License

BSD 3-Clause License
