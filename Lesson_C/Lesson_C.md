# Lesson C: Static range-and-bearing localization

## C1

Introducing the following constrains, we are able to solve this localization problem :
* ![formula](https://render.githubusercontent.com/render/math?math=[d]=y_1.\begin{Bmatrix}cos(\theta)\\%20\%20sin(\theta)\end{Bmatrix})
* ![formula](https://render.githubusercontent.com/render/math?math=d_1=m_1-x_1)
* ![formula](https://render.githubusercontent.com/render/math?math=d_2=m_2-x_2)
* ![formula](https://render.githubusercontent.com/render/math?math=\theta=x_3+y_2)

## C9

We are able to estimate the state of the robot, with the contractor network implemented with the previous constrains.

![](./images/static_localisation.png)
