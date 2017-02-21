
import java.awt.Graphics;

import java.awt.Color;

public class WidgetNumber implements Displayable, NetworkTableIO {


	private int x, y, w, h;

	private double value;
	private String name;
	private String location;//Relative to the /User/ network table

//----------------Constructor-----------------------//
	public WidgetNumber (double startingValue, String name, String locationToRead, int x, int y, int w, int h){
		value = startingValue;
		this.name = name;
		location = locationToRead;

		this.x = x;
		this.y = y;
		this.w = w;
		this.h = h;
	}//Most Specific Constructor
	public WidgetNumber (double startingValue, String locationToRead, int x, int y, int w, int h){
		value = startingValue;
		this.name = WidgetNumber.parseNameFromAbsolutePath(locationToRead);
		location = locationToRead;

		this.x = x;
		this.y = y;
		this.w = w;
		this.h = h;
	}//Less Specific Constructor


//-------------Interface Implementations--------------//
	public void setValue(/*NetworkTable userTable*/){
//		value = userTable.GetNumber(location);
	}
	public Object getValue (){
		return (Object) value;
	}

	public void render(Graphics g){
		g.setColor(Color.white);

		System.out.println("Rendering: "+name);
		g.fillRect(x,y,w,h);
		System.out.printf("X: %d, Y: %d, W: %d, H: %d\n",x,y,w,h);
		g.drawString(String.format("%30s == %10.10f",name,value),x+10,y+10);
	}


	public static String parseNameFromAbsolutePath(String absPath){
		/*
		 * Takes an absolute network table path (under /User/)
		 * and extracts the last token, for use as the WidgetNumber's
		 * screen name.
		 *
		 * Example: parseNameFromAbsolutePath("GearPixy/Frame") returns "Frame"
		 * 
		 */
		String lastToken = "";

		for (int i=0;i<absPath.length();i++){
			char c = absPath.charAt(i);
			if (c == '/'){
				lastToken = "";
			} else {
				lastToken += c;
			}
		}
		return lastToken;
	}//static parseNameFromAbsolutePath method
}//WidgetNumber class
