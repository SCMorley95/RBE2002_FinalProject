
class directionalArray {
public:
	directionalArray(int x_init, int y_init);
	void rotate(int deg);
	int xval();
	int yval();
private:
	int _array[2];
	int intSin(int deg);
	int intCos(int deg);
};