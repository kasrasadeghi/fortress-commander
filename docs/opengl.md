- instanced arrays and instanced uniforms seem to be equally fast
  - fps 60 (might be wrong)
- uniforms have a limit to how large they can be
- global uniforms
  - if a lot of shaders share a uniform, we should use global uniforms
  - global uniforms have a higher size limit


- how to draw
=> opengl-how-to-draw

- what is instancing
instancing kind of acts like a nested for loop
you say `glDrawArraysInstanced(GL\_TRIANGLES, first, count, instances)`
and it draws triangles using the vertex data from _first_, _count_ times

then for each instance, it passes along `gl\_InstanceID` to allow the shader to
update from uniforms, and it increments various attributes from buffers if they
have a non-zero divisor set by `glVertexAttribDivisor`
