# myPaint
A paint-like program developed in C and OpenGL that allows the user to create images. The
program features color selection, line drawing, color filling, brush size scaling, and saving the
final image in ".pgm" format.

Here is a demonstration of the execution:

https://user-images.githubusercontent.com/80704425/222125015-e932a092-f083-4998-a5b6-58729d4c5d85.mp4

## The Result
For now, the image is saved under the "vs2017test" directory and in a ".pgm" format. To convert the image, you can use additional conversion software or online service (I used [convertio.co](https://convertio.co/pgm-png/)). Here is the image from the demo!
<img src="https://user-images.githubusercontent.com/80704425/222126978-16aa1e21-7934-4734-9c9d-f4d49f09b0eb.png" width=50% height=50%>

After some practice, I believe I have even improved! 
<img src="https://user-images.githubusercontent.com/80704425/222127042-5d7d1fe8-a8ee-4e2d-b72c-bcfb526daba3.png" width=50% height=50%>
<img src="https://user-images.githubusercontent.com/80704425/222127074-f5de8f46-afee-436c-91e7-dd3f1fb039be.png" width=50% height=50%>
<img src="https://user-images.githubusercontent.com/80704425/222127060-8cd4ef1e-83b4-47ce-a844-eb68754951c9.png" width=50% height=50%>

## Features
By clicking the right mouse button, a menu opens with the following actions:  
**Save:** save the current image canvas.  
**Brush +:** increase the size of the drawing brush.  
**Brush -:** decrease the size of the drawing brush.  
**Fill:** fill the clicked area with a selected color.  
**Draw:** switch to drawing instead of filling areas.  
**Eyedrop:** select a color from a certain pixel in the canvas.  
**Restart:** clear the canvas.  

## Known Issues
As previously mentioned, the image is saved in a .pgm format, which is not convenient for the average user who simply wants to see the final result.  
Additionally, currently, the image is saved under the name "image.pgm". That means if multiple saving actions occur in one session, the images will override one another.  
<ins>For these reasons, I would like to add some additional features to the program:</ins>  
- Saving the pixel matrix in a png format.
- Allowing the users to save the image under a specific name chosen by them.
