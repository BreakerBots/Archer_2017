import java.util.List;
import java.util.ArrayList;

import java.awt.Graphics;
import java.awt.Color;
import javax.swing.JComponent;

public class WidgetPanel extends JComponent {

	List<Displayable> widgets;

	public WidgetPanel (int w, int h){
		super ();

		setBounds(10,10,w,h);
		setSize(w,h);
		setBackground(Color.yellow);

		widgets = new ArrayList<Displayable>();
	}

	public void add (Displayable widget){
		widgets.add(widget);
	}

	public void paint (Graphics g){
		System.out.println("Paint");
		System.out.printf("WidgetPanel: X: %d, Y: %d, Width: %d, Height: %d\n",getX(),getY(),getWidth(),getHeight());
		super.paintComponent(g);
		g.setColor(Color.yellow);
		g.fillRect(0,0,50,50);
		for (Displayable widget : widgets){
			widget.render(g);
		}
	}//paint method

}//WidgetPanel class
