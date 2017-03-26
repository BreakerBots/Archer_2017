//import networktables.NetworkTable;

import edu.wpi.first.wpilibj.networktables.*;

import java.util.List;
import java.util.ArrayList;

import javax.swing.JFrame;
import javax.swing.JPanel;
import java.awt.Color;

class Main {

	/*NetworkTable userTable;*/

	static List<NetworkTableIO> widgets = new ArrayList<NetworkTableIO>();

	public static void main(String[] args){
//		NetworkTables.SetClientMode();
//		NetworkTables.SetTeam(5104);
//		NetworkTable table = NetworkTable.GetTable("User");

		JFrame frame = new JFrame("BreakerDashboard");
		frame.setSize(3980,2000);
		frame.setBackground(Color.red);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);


		JPanel panel = new JPanel();
		panel.setBounds(10,10,3960,1980);
		panel.setBackground(Color.green);
		panel.setVisible(true);
		panel.setLayout(null);

		WidgetPanel widgetManager = new WidgetPanel(3940,1960);

		System.out.printf("1) WidgetPanel -- X: %d, Y: %d, W: %d, H: %d\n",widgetManager.getX(),widgetManager.getY(),widgetManager.getWidth(),widgetManager.getHeight());

		WidgetNumber valueFrame = new WidgetNumber(-1,"User/GearPixy/Frame",50,100,240,40);
		WidgetNumber valueNumOfObjects = new WidgetNumber(0,"User/GearPixy/NumOfObjects",50,150,240,40);

		widgetManager.add(valueFrame);
		widgets.add(valueFrame);
		widgetManager.add(valueNumOfObjects);
		widgets.add(valueNumOfObjects);

//		widgets.add(new WidgetNumberArray("User/GearPixy/Object0"));
//		widgets.add(new WidgetNumberArray("User/GearPixy/Object1"));
//		widgets.add(new WidgetNumberArray("User/GearPixy/Object2"));
//		widgets.add(new WidgetNumberArray("User/GearPixy/Object3"));
//		widgets.add(new WidgetNumberArray("User/GearPixy/Object4"));
//		widgets.add(new WidgetNumberArray("User/GearPixy/Object5"));
//		widgets.add(new WidgetNumberArray("User/GearPixy/Object6"));

		initComponents();

		panel.add(widgetManager);
		frame.add(panel);
		panel.repaint();
		frame.setVisible(true);
//		int x = 0;
//		while (true){
//			x += 1;
//			table.PutNumber("X",x);
//		}
	}

	public static void initComponents (){
		/*
		System.out.println("Loading Network Table Values");
		for (NetworkTableIO component : widgets){
			component.loadValue(/*userTable*//*);
		}
		*/
	}//initComponents method

}
