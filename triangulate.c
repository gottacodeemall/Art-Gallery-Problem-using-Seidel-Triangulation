//
//  triangulate.c
//  Triangulate
//
//  Created by Cameron Palmer on 05.10.14.
//  Copyright (c) 2014 NTNU. All rights reserved.
//

#include "seidel.h"

#include <stdio.h>
#include <stdlib.h>
#include "tri.h"
extern int numpoints;




int isSafe (int v, int graph[numpoints][numpoints], int color[], int c)
{
    for (int i = 0; i < numpoints; i++)
        if (graph[v][i] && c == color[i])
            return 0;
    return 1;
}
 
/* A recursive utility function to solve m coloring problem */
int threecolor(int graph[numpoints][numpoints], int m, int color[], int v)
{
    /* base case: If all vertices are assigned a color then
       return true */
    if (v == numpoints)
        return 1;
 
    /* Consider this vertex v and try different colors */
    for (int c = 1; c <= m; c++)
    {
        /* Check if assignment of color c to v is fine*/
        if (isSafe(v, graph, color, c))
        {
           color[v] = c;
 
           /* recur to assign colors to rest of the vertices */
           if (threecolor(graph, m, color, v+1) == 1)
             return 1;
 
            /* If assigning color c doesn't lead to a solution
               then remove it */
           color[v] = 0;
        }
    }
    /* If no color can be assigned to this vertex then return false */
    return 0;
}
 



int main(int argc, const char *argv[])
{
    int n, nmonpoly, genus;
    int op[SEGSIZE][3], i, ntriangles;

    if ((argc < 2) || ((n = read_segments(argv[1], &genus)) < 0)) {
        fprintf(stderr, "usage: triangulate <filename>\n");
        exit(1);
    }

    initialise(n);
    construct_trapezoids(n);
    nmonpoly = monotonate_trapezoids(n);
    ntriangles = triangulate_monotone_polygons(n, nmonpoly, op);
    

    int graph[numpoints][numpoints];
    for(int i=0;i<numpoints;i++){
        for(int j=0;j<numpoints;j++){
            graph[i][j]=0;
        }
    }

    FILE *fp;
    fp = fopen("triangulated", "w");
         fprintf(fp,"#!/usr/bin/gnuplot\n");
         fprintf(fp,"reset\n");
         fprintf(fp,"set terminal png\n");
         fprintf(fp,"set yrange [-2:30]\n");
         fprintf(fp,"set xrange [-2:30]\n");
         fprintf(fp,"unset colorbox\n");
         fprintf(fp,"set style arrow 1 nohead lc rgb \'black\'\n");
         fprintf(fp,"set style arrow 2 nohead lc rgb \'red\'\n");

    for (i = 0; i < ntriangles; i++) {
        printf("triangle #%d: %d %d %d\n", i, op[i][0], op[i][1], op[i][2]);
        fprintf(fp,"set arrow from %lf,%lf to %lf,%lf as 1\n",seg[op[i][0]].v0.x,seg[op[i][0]].v0.y,seg[op[i][1]].v0.x,seg[op[i][1]].v0.y);
        fprintf(fp,"set arrow from %lf,%lf to %lf,%lf as 1\n",seg[op[i][1]].v0.x,seg[op[i][1]].v0.y,seg[op[i][2]].v0.x,seg[op[i][2]].v0.y);
        fprintf(fp,"set arrow from %lf,%lf to %lf,%lf as 1\n",seg[op[i][0]].v0.x,seg[op[i][0]].v0.y,seg[op[i][2]].v0.x,seg[op[i][2]].v0.y);
        graph[op[i][0]-1][op[i][1]-1]=1;
        graph[op[i][1]-1][op[i][0]-1]=1;
        graph[op[i][1]-1][op[i][2]-1]=1;
        graph[op[i][2]-1][op[i][1]-1]=1;
        graph[op[i][0]-1][op[i][2]-1]=1;
        graph[op[i][2]-1][op[i][0]-1]=1;
    }
    fprintf(fp,"plot NaN notitle\n");
    fclose(fp);


    int color[numpoints];
    for(int i=0;i<numpoints;i++){
        color[i]=0;
    }
    threecolor(graph,3,color,0);
    int s1=0,s2=0,s3=0;
    for(int i=0;i<numpoints;i++){
        if(color[i]==1) s1++;
        else if(color[i]==2) s2++;
        else s3++;
    }
    printf("c1: %d c2: %d c3:%d \n",s1,s2,s3);
int index;
int ans;
  if(s1<s2&&s1<s3){
    index=0;  
    ans=s1;
  }
  else if(s2<s1&&s2<s3){
    index=1;
    ans=s2;  
  }
  else{
    index=2;
    ans=s3; 
  }
    FILE *fp2;
    fp2= fopen("org", "w");
    fp = fopen("guards", "w");
         fprintf(fp,"#!/usr/bin/gnuplot\n");
         fprintf(fp,"reset\n");
         fprintf(fp,"set terminal png\n");
         fprintf(fp,"set yrange [-2:30]\n");
         fprintf(fp,"set xrange [-2:30]\n");
         fprintf(fp,"unset colorbox\n");
         fprintf(fp,"set style arrow 1 nohead lc rgb \'black\'\n");
         fprintf(fp,"set style arrow 2 nohead lc rgb \'red\'\n");
         fprintf(fp2,"#!/usr/bin/gnuplot\n");
         fprintf(fp2,"reset\n");
         fprintf(fp2,"set terminal png\n");
         fprintf(fp2,"set yrange [-2:30]\n");
         fprintf(fp2,"set xrange [-2:30]\n");
         fprintf(fp2,"unset colorbox\n");
         fprintf(fp2,"set style arrow 1 nohead lc rgb \'black\'\n");
         fprintf(fp2,"set style arrow 2 nohead lc rgb \'red\'\n");
    for (i = 1; i <= numpoints; i++) {
        if(i==numpoints){
            fprintf(fp,"set arrow from %lf,%lf to %lf,%lf as 1\n",seg[i].v0.x,seg[i].v0.y,seg[1].v0.x,seg[1].v0.y);
            fprintf(fp2,"set arrow from %lf,%lf to %lf,%lf as 1\n",seg[i].v0.x,seg[i].v0.y,seg[1].v0.x,seg[1].v0.y);
        }
        else{
            fprintf(fp,"set arrow from %lf,%lf to %lf,%lf as 1\n",seg[i].v0.x,seg[i].v0.y,seg[i+1].v0.x,seg[i+1].v0.y);
            fprintf(fp2,"set arrow from %lf,%lf to %lf,%lf as 1\n",seg[i].v0.x,seg[i].v0.y,seg[i+1].v0.x,seg[i+1].v0.y);
        }
        if(index==color[i-1]){
            fprintf(fp,"set object circle at %lf,%lf size first 0.3 fc rgb 'navy'\n",seg[i].v0.x,seg[i].v0.y);
        }
    }
    fprintf(fp,"plot NaN notitle\n");
    fprintf(fp2,"plot NaN notitle\n");
    fclose(fp);
    fclose(fp2);

    fp = fopen("3color", "w");
         fprintf(fp,"#!/usr/bin/gnuplot\n");
         fprintf(fp,"reset\n");
         fprintf(fp,"set terminal png\n");
         fprintf(fp,"set yrange [-2:30]\n");
         fprintf(fp,"set xrange [-2:30]\n");
         fprintf(fp,"unset colorbox\n");
         fprintf(fp,"set style arrow 1 nohead lc rgb \'black\'\n");
         fprintf(fp,"set style arrow 2 nohead lc rgb \'red\'\n");
    for (i = 0; i < ntriangles; i++) {
        fprintf(fp,"set arrow from %lf,%lf to %lf,%lf as 1\n",seg[op[i][0]].v0.x,seg[op[i][0]].v0.y,seg[op[i][1]].v0.x,seg[op[i][1]].v0.y);
        fprintf(fp,"set arrow from %lf,%lf to %lf,%lf as 1\n",seg[op[i][1]].v0.x,seg[op[i][1]].v0.y,seg[op[i][2]].v0.x,seg[op[i][2]].v0.y);
        fprintf(fp,"set arrow from %lf,%lf to %lf,%lf as 1\n",seg[op[i][0]].v0.x,seg[op[i][0]].v0.y,seg[op[i][2]].v0.x,seg[op[i][2]].v0.y);
    }

    printf("Minimum number of guards required are: %d\n",ans);
    for (i = 1; i <= numpoints; i++) {
        if(color[i-1]==1){
            fprintf(fp,"set object circle at %lf,%lf size first 0.3 fc rgb 'navy'\n",seg[i].v0.x,seg[i].v0.y);
            printf("guard at %lf %lf\n",seg[i].v0.x,seg[i].v0.y);
        }
        else if(color[i-1]==2){
            fprintf(fp,"set object circle at %lf,%lf size first 0.3 fc rgb 'red'\n",seg[i].v0.x,seg[i].v0.y);
            printf("guard at %lf %lf\n",seg[i].v0.x,seg[i].v0.y);
        }
        else{
            fprintf(fp,"set object circle at %lf,%lf size first 0.3 fc rgb 'green'\n",seg[i].v0.x,seg[i].v0.y);
            printf("guard at %lf %lf\n",seg[i].v0.x,seg[i].v0.y);
        }
    }
    fprintf(fp,"plot NaN notitle\n");
    fclose(fp);
    return 0;
}
