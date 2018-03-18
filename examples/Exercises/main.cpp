#include "Exercises.h"

Exercises::Exercises() {
	Add(mc.SizePos());
	
	Title("University Exercises");
	Sizeable().MaximizeBox().MinimizeBox();
	
}

void Exercises::HelloWorld() {
	Node a = Node("A") * Node(5);
	Node b = Node("Y") == Pow2(Node("X"));
	
	Node c = b;
	c.Solve(Node("X"), true);
	
	Node d = b[0] == b[1] + Node("X") * Node("X");
	Node e = d;
	e[1].Simplify();
	
	mc.Inner("Sub test 1");
	mc.Print("A", a);
	mc.Print("B", b);
	mc.Print("C", c);
	mc.Outer();
	
	mc.Inner("Sub test 2");
	mc.Print("D", d);
	mc.Print("E", e);
	mc.Outer();
	
	
	LOG(b.AsString());
	LOG(b.AsDataInlineString());
	LOG(b.AsInlineString());
}


void Exercises::ComplexAnalysis1() {
	
	{
		Node n0 = Node("z") == (Pow2(Node(-1) + Node("i")) == Pow2(Node("w")));
		Node n1 = n0[1][0];
		n1.Simplify();
		Node real = Real(n1);
		Node imag = Imag(n1);
		Node abs = Abs(n1);
		Node arg = Arg(n1);
		abs.Simplify();
		
		{
			mc.Inner("Define real part, imaginary part and absolute value");
			mc.Print("Input", n0);
			mc.Print("Input", n1);
			mc.Print("Real part: Re z =", real);
			mc.Print("Imag part: Im z =", imag);
			mc.Print("Absolute: |z| =", abs);
			mc.Outer();
		}
		
		{
			mc.Inner("Exponent form");
			mc.Print("1", n0);
			mc.Print("Arg part: arg(z) =", arg);
			
			Node n2 = Node("z") == Abs(Node("z")) * (Node("e") ^ (Node("i") * Node("arg(z)")));
			mc.Print("Form", n2);
			
			Node n3 = Node("z") == abs * (Node("e") ^ (Node("i") * arg));
			mc.Print("Form", n3);
			
			n3[1].Simplify();
			mc.Print("Answer", n3);
			
			mc.Outer();
		}
	}
	
	{
		mc.Inner("Define absolute value and argument");
		Node n0 = ParseExpression("z = ((sqrt(3))-(i))/(1+(sqrt(3))*(i))");
		mc.Print("Input", n0);
		
		Node abs = Abs(n0[1]);
		Node arg = Arg(n0[1]);
		mc.Print("Absolute: |z| =", abs);
		mc.Print("Arg part: arg(z) =", arg);
		
		mc.Outer();
	}
	
	{
		mc.Inner("Define absolute value and argument");
		Node n0 = ParseExpression("z_1 = (-1+i)^7");
		mc.Print("Input", n0);
		Node n1 = ParseExpression("z_2 = (-1+i)");
		mc.Print("Use", n1);
		
		Node abs = Abs(n1[1]);
		Node arg = Arg(n1[1]);
		abs.Simplify();
		arg.Simplify();
		mc.Print("Absolute: |z_2| =", abs);
		mc.Print("Arg part: arg(z_2) =", arg);
		
		Node n3 = Node("z_2") == abs * (Node("e") ^ (Node("i") * arg));
		mc.Print("Form", n3);
		
		Node n4 = Node("z_1") == ((abs * (Node("e") ^ (Node("i") * arg))) ^ Node(7));
		mc.Print("Simplify", n4);
		
		n4[1].Simplify();
		mc.Print("Get abs and arg", n4);
		
		abs = Abs(n4[1]);
		arg = Arg(n4[1]);
		abs.Simplify();
		arg.Simplify();
		mc.Print("Absolute: |z_1| =", abs);
		mc.Print("Arg part: arg(z_1) =", arg);
		
		mc.Outer();
	}
	
	{
		mc.Inner("Find cube root");
		
		Node n0 = ParseExpression("w = -8*i");
		mc.Print("Input", n0);
		
		mc.Print("Find", ParseExpression("w^(1/3)"));
		mc.Print("Let", ParseExpression("w = z^3"));
		mc.Print("Let", ParseExpression("z = r_z*e^(i*omega_z)"));
		mc.Print("Let", ParseExpression("w = r_w*e^(i*omega_w)"));
		
		Node abs = Abs(n0[1]);
		Node arg = Arg(n0[1]);
		abs.Simplify();
		arg.Simplify();
		mc.Print("Absolute: |w| =", abs);
		mc.Print("Arg part: arg(w) =", arg);
		
		Node n1 = (Node("z") ^3) == (abs * (Node("e") ^ (Node("i") * arg)));
		mc.Print("Solve z", n1);
		
		Node n2 = n1;
		n2.SolveComplex(Node("z"), true);
		mc.Print("Cube root", n2);
		
		
		mc.Outer();
	}
	
	/*{
		mc.Inner("Find root");
		
		Node n0 = ParseExpression("z^4+4=0");
		mc.Print("Input", n0);
		
		
		
		mc.Outer();
	}*/
}

GUI_APP_MAIN {
	Exercises ue;
	
	//ue.HelloWorld();
	ue.ComplexAnalysis1();
	
	ue.mc.Refresh();
	
	ue.Run();
}

