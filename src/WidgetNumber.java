
import java.awt.Graphics;

import edu.wpi.first.wpilibj.networktables.NetworkTable;

import java.awt.Color;
import java.awt.Font;

public class WidgetNumber implements Displayable, NetworkTableIO {

	public static Font font = new Font ("Courier New",1,30);
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
	public void setValue(NetworkTable userTable){
		value = userTable.getNumber(location);
	}
	public Object getValue (){
		return (Object) value;
	}

	public void render(Graphics g){
		g.setColor(Color.white);
		g.fillRect(x,y,w,h);

		g.setFont(font);
		g.setColor(Color.blue);
		g.drawString(String.format("%15s == %.6f",name,value),x-30,y+27);

//		System.out.println("Rendering: "+name);
//		System.out.printf("X: %d, Y: %d, W: %d, H: %d\n",x,y,w,h);
		
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
