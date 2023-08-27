#pragma once

class Noise
{
private:
	static constexpr double F2 = 0.36602540378;
	static constexpr double G2 = 0.2113248654;

	static constexpr unsigned char simplexPerm[512]{
		151ui8,160ui8,137ui8,91ui8,90ui8,15ui8,131ui8,13ui8,201ui8,95ui8,96ui8,53ui8,194ui8,233ui8,7ui8,225ui8,140ui8,36ui8,103ui8,30ui8,
		69ui8,142ui8,8ui8,99ui8,37ui8,240ui8,21ui8,10ui8,23ui8,190ui8,6ui8,148ui8,247ui8,120ui8,234ui8,75ui8,0ui8,26ui8,197ui8,62ui8,94ui8,
		252ui8,219ui8,203ui8,117ui8,35ui8,11ui8,32ui8,57ui8,177ui8,33ui8,88ui8,237ui8,149ui8,56ui8,87ui8,174ui8,20ui8,125ui8,136ui8,
		171ui8,168ui8,68ui8,175ui8,74ui8,165ui8,71ui8,134ui8,139ui8,48ui8,27ui8,166ui8,77ui8,146ui8,158ui8,231ui8,83ui8,111ui8,229ui8,
		122ui8,60ui8,211ui8,133ui8,230ui8,220ui8,105ui8,92ui8,41ui8,55ui8,46ui8,245ui8,40ui8,244ui8,102ui8,143ui8,54ui8,65ui8,25ui8,63ui8,
		161ui8,1ui8,216ui8,80ui8,73ui8,209ui8,76ui8,132ui8,187ui8,208ui8,89ui8,18ui8,169ui8,200ui8,196ui8,135ui8,130ui8,116ui8,188ui8,
		159ui8,86ui8,164ui8,100ui8,109ui8,198ui8,173ui8,186ui8,3ui8,64ui8,52ui8,217ui8,226ui8,250ui8,124ui8,123ui8,5ui8,202ui8,38ui8,
		147ui8,118ui8,126ui8,255ui8,82ui8,85ui8,212ui8,207ui8,206ui8,59ui8,227ui8,47ui8,16ui8,58ui8,17ui8,182ui8,189ui8,28ui8,42ui8,223ui8,
		183ui8,170ui8,213ui8,119ui8,248ui8,152ui8,2ui8,44ui8,154ui8,163ui8,70ui8,221ui8,153ui8,101ui8,155ui8,167ui8,43ui8,172ui8,9ui8,
		129ui8,22ui8,39ui8,253ui8,19ui8,98ui8,108ui8,110ui8,79ui8,113ui8,224ui8,232ui8,178ui8,185ui8,112ui8,104ui8,218ui8,246ui8,97ui8,
		228ui8,251ui8,34ui8,242ui8,193ui8,238ui8,210ui8,144ui8,12ui8,191ui8,179ui8,162ui8,241ui8,81ui8,51ui8,145ui8,235ui8,249ui8,14ui8,
		239ui8,107ui8,49ui8,192ui8,214ui8,31ui8,181ui8,199ui8,106ui8,157ui8,184ui8,84ui8,204ui8,176ui8,115ui8,121ui8,50ui8,45ui8,127ui8,
		4ui8,150ui8,254ui8,138ui8,236ui8,205ui8,93ui8,222ui8,114ui8,67ui8,29ui8,24ui8,72ui8,243ui8,141ui8,128ui8,195ui8,78ui8,66ui8,215ui8,
		61ui8,156ui8,180ui8,151ui8,160ui8,137ui8,91ui8,90ui8,15ui8,131ui8,13ui8,201ui8,95ui8,96ui8,53ui8,194ui8,233ui8,7ui8,225ui8,140ui8,
		36ui8,103ui8,30ui8,69ui8,142ui8,8ui8,99ui8,37ui8,240ui8,21ui8,10ui8,23ui8,190ui8,6ui8,148ui8,247ui8,120ui8,234ui8,75ui8,0ui8,26ui8,197ui8,
		62ui8,94ui8,252ui8,219ui8,203ui8,117ui8,35ui8,11ui8,32ui8,57ui8,177ui8,33ui8,88ui8,237ui8,149ui8,56ui8,87ui8,174ui8,20ui8,125ui8,
		136ui8,171ui8,168ui8,68ui8,175ui8,74ui8,165ui8,71ui8,134ui8,139ui8,48ui8,27ui8,166ui8,77ui8,146ui8,158ui8,231ui8,83ui8,111ui8,
		229ui8,122ui8,60ui8,211ui8,133ui8,230ui8,220ui8,105ui8,92ui8,41ui8,55ui8,46ui8,245ui8,40ui8,244ui8,102ui8,143ui8,54ui8,65ui8,25ui8,
		63ui8,161ui8,1ui8,216ui8,80ui8,73ui8,209ui8,76ui8,132ui8,187ui8,208ui8,89ui8,18ui8,169ui8,200ui8,196ui8,135ui8,130ui8,116ui8,188ui8,
		159ui8,86ui8,164ui8,100ui8,109ui8,198ui8,173ui8,186ui8,3ui8,64ui8,52ui8,217ui8,226ui8,250ui8,124ui8,123ui8,5ui8,202ui8,38ui8,147ui8,
		118ui8,126ui8,255ui8,82ui8,85ui8,212ui8,207ui8,206ui8,59ui8,227ui8,47ui8,16ui8,58ui8,17ui8,182ui8,189ui8,28ui8,42ui8,223ui8,183ui8,
		170ui8,213ui8,119ui8,248ui8,152ui8,2ui8,44ui8,154ui8,163ui8,70ui8,221ui8,153ui8,101ui8,155ui8,167ui8,43ui8,172ui8,9ui8,129ui8,22ui8,
		39ui8,253ui8,19ui8,98ui8,108ui8,110ui8,79ui8,113ui8,224ui8,232ui8,178ui8,185ui8,112ui8,104ui8,218ui8,246ui8,97ui8,228ui8,251ui8,
		34ui8,242ui8,193ui8,238ui8,210ui8,144ui8,12ui8,191ui8,179ui8,162ui8,241ui8,81ui8,51ui8,145ui8,235ui8,249ui8,14ui8,239ui8,107ui8,
		49ui8,192ui8,214ui8,31ui8,181ui8,199ui8,106ui8,157ui8,184ui8,84ui8,204ui8,176ui8,115ui8,121ui8,50ui8,45ui8,127ui8,4ui8,150ui8,254ui8,
		138ui8,236ui8,205ui8,93ui8,222ui8,114ui8,67ui8,29ui8,24ui8,72ui8,243ui8,141ui8,128ui8,195ui8,78ui8,66ui8,215ui8,61ui8,156ui8,180ui8
	};

	static constexpr signed char simplexGrad2[12][2]{
		{1i8, 1i8},{-1i8, 1i8},{ 1i8,-1i8},{-1,-1i8},
		{1i8, 0i8},{-1i8, 0i8},{ 1i8, 0i8},{-1, 0i8},
		{0i8, 1i8},{ 0i8,-1i8},{ 0i8, 1i8},{ 0,-1i8}
	};

	static __forceinline double Floor(double n) { return n >= 0.0f ? (long long)n : (long long)n - 1; }
	static __forceinline double Dot(const signed char g[2], double x, double y) { return g[0] * x + g[1] * y; }

public:
	static double Simplex2(double x, double y)
	{
		double n0, n1, n2;
		double g = (x + y) * F2;
		long long i = Floor(x + g);
		long long j = Floor(y + g);

		double t = (i + j) * G2;
		double X0 = i - t;
		double Y0 = j - t;
		double x0 = x - X0;
		double y0 = y - Y0;

		long long i1, j1;
		if (x0 > y0) { i1 = 1; j1 = 0; }
		else { i1 = 0; j1 = 1; }

		double x1 = x0 - i1 + G2;
		double y1 = y0 - j1 + G2;
		double x2 = x0 - 1.0 + 2.0 * G2;
		double y2 = y0 - 1.0 + 2.0 * G2;

		long long ii = i & 255;
		long long jj = j & 255;
		long long gi0 = simplexPerm[ii + simplexPerm[jj]] % 12;
		long long gi1 = simplexPerm[ii + i1 + simplexPerm[jj + j1]] % 12;
		long long gi2 = simplexPerm[ii + 1 + simplexPerm[jj + 1]] % 12;
		double t0 = 0.5 - x0 * x0 - y0 * y0;

		if (t0 < 0) { n0 = 0.0; }
		else
		{
			t0 *= t0;
			n0 = t0 * t0 * Dot(simplexGrad2[gi0], x0, y0);
		}

		double t1 = 0.5 - x1 * x1 - y1 * y1;

		if (t1 < 0) { n1 = 0.0; }
		else
		{
			t1 *= t1;
			n1 = t1 * t1 * Dot(simplexGrad2[gi1], x1, y1);
		}

		double t2 = 0.5 - x2 * x2 - y2 * y2;

		if (t2 < 0) { n2 = 0.0; }
		else
		{
			t2 *= t2;
			n2 = t2 * t2 * Dot(simplexGrad2[gi2], x2, y2);
		}

		return 70.0 * (n0 + n1 + n2);
	}
};