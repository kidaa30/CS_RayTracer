
/* 
 * Auteur Pierre Froumenty
 * Classe qui permet gérer l'octree
 * utilisant l'intersection entre box et triangle (Mike Vandelay, http://www.planet-source-code.com/vb/scripts/ShowCode.asp?txtCodeId=10317&lngWId=3, 2006)
 */

#include <octree/Octree.h>
#include <octree/tritest.cpp>

Octree::Octree(Scene* scene)
{
    
    // init
    m_scene = scene; // scene
	m_max_level = 1; // criteres d'arret
	m_objects_max = 1;  // criteres d'arret
	m_level = 0; // niveau actuel
	m_objects_number = m_scene->m_triangles.size() + m_scene->m_planes.size() + m_scene->m_quadrics.size(); // nombre d'objets dans la scene
	m_nb_prim_max = 0; // nombre max de primitives trouvees dans un noeud (pour le passage des donnees au glsl)
	
    // Calcul de la boite englobante de la scene
	for (int i =0; i< m_scene->m_triangles.size(); i++){
		if (i==0) {
			m_xmin = m_scene->m_triangles[i].p0.x; 
			m_xmax = m_scene->m_triangles[i].p0.x; 
			m_ymin = m_scene->m_triangles[i].p0.y; 
			m_ymax = m_scene->m_triangles[i].p0.y; 
			m_zmin = m_scene->m_triangles[i].p0.z; 
			m_zmax = m_scene->m_triangles[i].p0.z; 
		}
		m_xmin = std::min(std::min(std::min(m_xmin,float(m_scene->m_triangles[i].p0.x)),float(m_scene->m_triangles[i].p1.x)),float(m_scene->m_triangles[i].p2.x));
		m_ymin = std::min(std::min(std::min(m_ymin,float(m_scene->m_triangles[i].p0.y)),float(m_scene->m_triangles[i].p1.y)),float(m_scene->m_triangles[i].p2.y));
		m_zmin = std::min(std::min(std::min(m_zmin,float(m_scene->m_triangles[i].p0.z)),float(m_scene->m_triangles[i].p1.z)),float(m_scene->m_triangles[i].p2.z));
		m_xmax = std::max(std::max(std::max(m_xmax,float(m_scene->m_triangles[i].p0.x)),float(m_scene->m_triangles[i].p1.x)),float(m_scene->m_triangles[i].p2.x));
		m_ymax = std::max(std::max(std::max(m_ymax,float(m_scene->m_triangles[i].p0.y)),float(m_scene->m_triangles[i].p1.y)),float(m_scene->m_triangles[i].p2.y));
		m_zmax = std::max(std::max(std::max(m_zmax,float(m_scene->m_triangles[i].p0.z)),float(m_scene->m_triangles[i].p1.z)),float(m_scene->m_triangles[i].p2.z));
	}
	m_sizeX = m_xmax - m_xmin;
	m_sizeY = m_ymax - m_ymin;
	m_sizeZ = m_zmax - m_zmin;
	
	// construction de l'octree
	m_root = new Node(); //Noeud racine
	m_nodes.push_back(*m_root);
	Octree::build(0,m_xmin,m_ymin,m_zmin,m_xmax,m_ymax,m_zmax); // construire l'octree
	
}


// Fonction qui verifie si un triangle est contenu partiellement ou totalement dans un noeud
bool Octree::isTriangleInNode(Triangle triangle,float xmin,float ymin,float zmin,float xmax,float ymax,float zmax){

	float zm = (zmin + zmax)/2.0; // milieu
	float ym = (ymin + ymax)/2.0; // milieu
	float xm = (xmin + xmax)/2.0; // milieu
	float center[3] = {xm, ym, zm};
	float r[3] = {xmax-xmin, ymax-ymin, zmax-zmin};
	float triverts[3][3] = {
	{triangle.p0.x,triangle.p0.y, triangle.p0.z},
	{triangle.p1.x,triangle.p1.y, triangle.p1.z},
	{triangle.p2.x,triangle.p2.y, triangle.p2.z}
	};

	bool intersection = tribox::isectboxtri(center, r, triverts);
	//std::cout << intersection << std::endl;
	return intersection;
}



void Octree::build(int cur_node_id, float xmin,float ymin,float zmin,float xmax,float ymax,float zmax)
{
	

	int objects_id_counter = 0;
	int local_objects_number = 0; 

	
	// Calcul du Nombre d'objets dans le noeud
	for (int i =0; i< m_scene->m_triangles.size(); i++){
		if ( isTriangleInNode(m_scene->m_triangles[i],xmin,ymin,zmin,xmax,ymax,zmax)){
			local_objects_number++;
			m_nodes[cur_node_id].objects_id.push_back(i);
		}
	}
	
	
	// coordonnees du voxel/noeud
	m_nodes[cur_node_id].coords[0] = xmin;
	m_nodes[cur_node_id].coords[1] = ymin;
	m_nodes[cur_node_id].coords[2] = zmin;
	m_nodes[cur_node_id].coords[3] = xmax;
	m_nodes[cur_node_id].coords[4] = ymax;
	m_nodes[cur_node_id].coords[5] = zmax;

	
	if ( local_objects_number > m_objects_max && m_level<m_max_level) { // si le critere d'arret nest pas atteint : NOEUD INTERNE
		m_level++; // niveau de profondeur
		for (int i = 0; i<8;i++) { // creer 8 fils pour le noeud courant
			
			// ajouter le nouveau noeud enfant au vecteur de noeuds
			int child_id = m_nodes.size();
			m_nodes[cur_node_id].child[i] = child_id;
			Node* new_node = new Node();
			m_nodes.push_back(*new_node); 
			
			// milieu
			float zm = (zmin + zmax)/2.0; // milieu
			float ym = (ymin + ymax)/2.0; // milieu
			float xm = (xmin + xmax)/2.0; // milieu
			
			// parcours des enfants
			switch (i){
				case 0: {
					//std::cout << "enfant 0" << std::endl;
					build(child_id, xmin, ymin, zmin, xm, ym, zm);
					break;
				}
				case 1: {
					//std::cout << "enfant 1" << std::endl;
					build(child_id, xmin, ymin, zm, xm, ym, zmax);
					break;
				}
				case 2: {
					//std::cout << "enfant 2" << std::endl;
					build(child_id, xmin, ym, zmin, xm, ymax, zm);
					break;
				}
				case 3: {
					//std::cout << "enfant 3" << std::endl;
					build(child_id, xmin, ym, zm, xm, ymax, zmax);
					break;
				}
				case 4: {
					//std::cout << "enfant 4" << std::endl;
					build(child_id, xm, ymin, zmin, xmax, ym, zm);
					break;
				}
				case 5: {
					//std::cout << "enfant 5" << std::endl;
					build(child_id, xm, ymin, zm, xmax, ym, zmax);
					break;
				}
				case 6: {
					//std::cout << "enfant 6" << std::endl;
					build(child_id, xm, ym, zmin, xmax, ymax, zm);
					break;
				}
				case 7: {
					//std::cout << "enfant 7" << std::endl;
					build(child_id, xm, ym, zm, xmax, ymax, zmax);
					break;
				}
			}
		}
		m_level--; // decrementer le niveau de profondeur
		return;
	}
	else { 
		//std::cout << "le noeud est terminal" << std::endl; // NOEUD TERMINAL
		for (int i = 0; i<8;i++) {m_nodes[cur_node_id].child[i] = -1;} // enfants a -1 si le noeud est terminal
		m_nb_prim_max = std::max(m_nb_prim_max,int(m_nodes[cur_node_id].objects_id.size())); // mise a jour du nombre max de primitives trouvees dans un noeud
		return;
	}
    
}

Octree::~Octree()
{
    
}
