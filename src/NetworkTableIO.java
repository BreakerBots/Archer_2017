import edu.wpi.first.wpilibj.networktables.NetworkTable;

public interface NetworkTableIO {

	public void setValue(NetworkTable nt);
	public Object getValue();

}
