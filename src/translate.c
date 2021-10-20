/* Shell ISO8859-7 to ISO8859-1 translator
 *
 * @author: Antonakakis Alexandros <csd4802>
 *
 * Converts ISO8859-7 characters to ISO8859-1 characters
 *
 * Revised February 21st 2021: Removed global 2darray set_map
 *
 */

#include <stdio.h>
#include <stddef.h>

#define SET_SIZE 255

#define accent 39
#define diaeresis 34

#define A_gr 193
#define a_gr 225
#define E_gr 197
#define e_gr 229
#define O_gr 207
#define o_gr 239
#define I_gr 201
#define i_gr 233
#define H_gr 199
#define h_gr 231
#define Y_gr 213
#define y_gr 245
#define M_gr 204
#define m_gr 236
#define N_gr 205
#define n_gr 237
#define T_gr 212
#define t_gr 244
#define P_gr 208
#define p_gr 240
#define W_gr 217
#define w_gr 249
#define Ks_gr 206
#define ks_gr 238
#define Ps_gr 216
#define ps_gr 248
#define K_gr 202
#define k_gr 234
#define S_gr 211
#define s_gr 242

#define B_en 66
#define b_en 98
#define W_en 127
#define w_en 167
#define M_en 77
#define m_en 109
#define N_en 78
#define n_en 110
#define T_en 84
#define t_en 116
#define D_en 68
#define d_en 100

#define A_acc_gr 182
#define a_acc_gr 220
#define E_acc_gr 184
#define e_acc_gr 221
#define O_acc_gr 188
#define o_acc_gr 252
#define I_acc_gr 186
#define i_acc_gr 223
#define Y_acc_gr 190
#define y_acc_gr 253
#define W_acc_gr 191
#define w_acc_gr 254
#define H_acc_gr 185
#define h_acc_gr 222

#define I_dia_gr 218
#define i_dia_gr 250
#define Y_dia_gr 219
#define y_dia_gr 251

#define i_dia_acc_gr 192
#define y_dia_acc_gr 224

enum state
{
	dflt=0,state_m_gr,state_M_gr,state_n_gr,state_N_gr  /* state machine with 5 states */
};

void usage(void)
{
	fprintf(stderr,"ISO8859-7 to ISO8859-1 character conversion\n");

	return;
}

void initialize_map(int set_map[][3])
{
	char *cap_gr = "AVGDEZH8IKLMNQOPR$STYFXYW";
	char *low_gr = "avgdezh8iklmnqoprsstyfxyw";
	size_t i = 0U,j = 0U;

	for (; i < 193; i++) /* Mapping regular characters */
	{
		set_map[i][0] = i;
	}

	for (i = 193; i < 218; i++) /* Mapping uppercase */
	{
		set_map[i][0] = cap_gr[j++];
	}

	for (i = 225,j = 0; i < 250; i++) /* Mapping lowercase */
	{
		set_map[i][0] = low_gr[j++];
	}

	return;
}

void map_special(int set_map[][3])
{
	/* Special characters psi and ksi */

	set_map[Ks_gr][0] = set_map[K_gr][0]; set_map[Ks_gr][1] = set_map[S_gr][0];
	set_map[ks_gr][0] = set_map[k_gr][0]; set_map[ks_gr][1] = set_map[s_gr][0];
	set_map[Ps_gr][0] = set_map[P_gr][0]; set_map[Ps_gr][1] = set_map[S_gr][0];
	set_map[ps_gr][0] = set_map[p_gr][0]; set_map[ps_gr][1] = set_map[s_gr][0];

	/* Upper case letters with greek tonos */
	set_map[A_acc_gr][0] = accent; set_map[A_acc_gr][1] = set_map[A_gr][0];
	set_map[E_acc_gr][0] = accent; set_map[E_acc_gr][1] = set_map[E_gr][0];
	set_map[O_acc_gr][0] = accent; set_map[O_acc_gr][1] = set_map[O_gr][0];
	set_map[W_acc_gr][0] = accent; set_map[W_acc_gr][1] = set_map[W_gr][0];
	set_map[I_acc_gr][0] = accent; set_map[I_acc_gr][1] = set_map[I_gr][0];
	set_map[Y_acc_gr][0] = accent; set_map[Y_acc_gr][1] = set_map[Y_gr][0];
	set_map[H_acc_gr][0] = accent; set_map[H_acc_gr][1] = set_map[H_gr][0];

	/* Lower case letters with greek tonos */
	set_map[a_acc_gr][0] = set_map[a_gr][0]; set_map[a_acc_gr][1] = accent;
	set_map[e_acc_gr][0] = set_map[e_gr][0]; set_map[e_acc_gr][1] = accent;
	set_map[o_acc_gr][0] = set_map[o_gr][0]; set_map[o_acc_gr][1] = accent;
	set_map[w_acc_gr][0] = set_map[w_gr][0]; set_map[w_acc_gr][1] = accent;
	set_map[i_acc_gr][0] = set_map[i_gr][0]; set_map[i_acc_gr][1] = accent;
	set_map[y_acc_gr][0] = set_map[y_gr][0]; set_map[y_acc_gr][1] = accent;
	set_map[h_acc_gr][0] = set_map[h_gr][0]; set_map[h_acc_gr][1] = accent;

	/* Letters with greek dialitika */
	set_map[I_dia_gr][0] = set_map[I_gr][0]; set_map[I_dia_gr][1] = diaeresis;
	set_map[i_dia_gr][0] = set_map[i_gr][0]; set_map[i_dia_gr][1] = diaeresis;
	set_map[Y_dia_gr][0] = set_map[Y_gr][0]; set_map[Y_dia_gr][1] = diaeresis;
	set_map[y_dia_gr][0] = set_map[y_gr][0]; set_map[y_dia_gr][1] = diaeresis;

	/* Greek letters with greek dialitika and greek tonos */

	set_map[i_dia_acc_gr][0] = set_map[i_gr][0];
	set_map[i_dia_acc_gr][1] = accent;
	set_map[i_dia_acc_gr][2] = diaeresis;
	set_map[y_dia_acc_gr][0] = set_map[y_gr][0];
	set_map[y_dia_acc_gr][1] = accent;
	set_map[y_dia_acc_gr][2] = diaeresis;

	return;
}

/* Prints the corresping ISO8859-1 character */

void print_def(int c,int set_map[][3])
{
	size_t i = 0U;

	for (; i < 3; i++)
	{
		if (set_map[c][i]) /* set_map is initialized wth zeros, changed if needed */
		{
			fputc(set_map[c][i],stdout);
		}
	}

	return;
}

enum state state_def(int c,int set_map[][3])
{
	switch (c) /* checking whether c is a special case or a normal character */
	{
		case M_gr:
			return state_M_gr;
		case m_gr:
			return state_m_gr;
		case N_gr:
			return state_N_gr;
		case n_gr:
			return state_n_gr;
		default:
			print_def(c,set_map);
			return dflt;
	}
}

/* Functions to handle special cases */

enum state state_M(int c,int set_map[][3])
{
	switch (c)
	{
		case P_gr:
		case p_gr:
			fputc(B_en,stdout);
			return dflt;
		case M_gr:
			fputc(M_en,stdout);
			return state_M_gr;
		default: /* Character read was not of interest */
			fputc(M_en,stdout);
			print_def(c,set_map);
			return dflt;
	}
}

enum state state_m(int c,int set_map[][3])
{
	switch (c)
	{
		case P_gr:
		case p_gr:
			fputc(b_en,stdout);
			return dflt;
		case m_gr:
			fputc(m_en,stdout);
			return state_m_gr;
		default:
			fputc(m_en,stdout);
			print_def(c,set_map);
			return dflt;
	}
}


enum state state_N(int c,int set_map[][3])
{
	switch (c)
	{
		case T_gr:
		case t_gr:
			fputc(D_en,stdout);
			return dflt;
		case N_gr:
			fputc(N_en,stdout);
			return state_N_gr;
		default:
			fputc(N_en,stdout);
			print_def(c,set_map);
			return dflt;
	}
}

enum state state_n(int c,int set_map[][3])
{
	switch (c)
	{
		case T_gr:
		case t_gr:
			fputc(d_en,stdout);
			return dflt;
		case n_gr:
			fputc(n_en,stdout);
			return state_n_gr;
		default:
			fputc(n_en,stdout);
			print_def(c,set_map);
			return dflt;
	}
}

enum state (*functions[])(int c,int set_map[][3]) = { state_def,state_m,state_M,state_n,state_N }; /* array of functions (bonus) */

int main(void)
{
	int c = 0;
	int set_map[SET_SIZE][3] = {{0}};

	enum state fsm_state = dflt;

	initialize_map(set_map); /* Initilizing the map */
	map_special(set_map); /* Mapping special characters */

	while ((c = fgetc(stdin)) != EOF)
	{
		fsm_state = (functions[fsm_state](c,set_map));
	}

	return 0;
}

