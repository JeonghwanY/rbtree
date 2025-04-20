#include "rbtree.h"
#include <stdlib.h>
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t*)calloc(1,sizeof(node_t));
  nil->color=RBTREE_BLACK;
  nil->left=nil;
  nil->right=nil;
  nil->parent=nil;
  p->root=nil;
  p->nil=nil;
 
  return p;
}
static void postorder_free(rbtree *t, node_t *x){
    if (x == t->nil) return;
    postorder_free(t,x->left);
    postorder_free(t,x->right);
    free(x);
        
}
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
    if(!t) return;
    postorder_free(t,t->root);
    free(t->nil);
    free(t);
}
void left_rotate(rbtree*t,node_t*x){
     node_t *y=x->right; //y에 x의 right값을 넣어줌
     x->right=y->left;// 회전할거니까 이때 y의 왼쪽값을 x의 오른쪽에 넣어준다
     if (y->left!=t->nil)//만약 y left가 값이 있을때
         y->left->parent=x;//이동한것이기 때문에 부모위치도 재조정해준다.
     y->parent=x->parent;//회전시킨거니까 y의 부모는 기존 x의 부모로 재조정해준다
     if(x->parent==t->nil)//만약 x가 루트였다면
         t->root=y;//트리의 루트는 y로 설정
     else if(x==x->parent->left)//x가 부모의 왼쪽자식이였을때
         x->parent->left=y;//y도 왼쪽자식으로 들어간다
     else//이건 x가 오른쪽자식이였을때
         x->parent->right=y;//x가 있던 자리를 y가 차지한다
     y->left=x;//y의 왼쪽은 x가 들어온다
     x->parent=y;//x의 부모는 y가 된다.
 } 
void right_rotate(rbtree*t,node_t*x){
     node_t *y=x->left;
     x->left=y->right;
     if(y->right!=t->nil)//nil이면 부모설정이 필요없으니까
         y->right->parent=x;
     y->parent=x->parent;
     if(x->parent==t->nil)//x가 루트였으니 y도 루트
         t->root=y;
     else if(x==x->parent->right)//x가 부모의 왼쪽값이여서 y도 동일하게 설정
         x->parent->right=y;
     else
         x->parent->left=y;
     y->right=x;//여기가 어디로 회전할지 명시해주는 곳
     x->parent=y;//x의 부모는 y로 설정


 }
void insert_fixup(rbtree*t,node_t*newnode){
     while(newnode->parent->color==RBTREE_RED){
	 if(newnode->parent==t->nil || newnode->parent->parent==t->nil) break; 
         if(newnode->parent==newnode->parent->parent->left){//newnode의 부모가 조부모의 왼쪽자식이라면
             node_t *y = newnode->parent->parent->right;//y가 삼촌노드
             if (y->color==RBTREE_RED){//삼촌노드도 빨간색이면 case1
                 //case 1 부모와 삼촌의 컬러를 바꾸고 조부모로 올라감.
                 newnode->parent->color=RBTREE_BLACK;
                 y->color=RBTREE_BLACK;
                 newnode->parent->parent->color=RBTREE_RED;
                 newnode=newnode->parent->parent;//위에도 확인해야 하기때문에 할아버지노드로 바꿔준다.
             }else{//case 2 꺾여잇을때 자식와 부모를  부모기준으로 회전.
                 if(newnode==newnode->parent->right){
                     newnode=newnode->parent;
                     left_rotate(t,newnode);
                 }
                 //case3.
                 newnode->parent->color=RBTREE_BLACK;
                 newnode->parent->parent->color=RBTREE_RED;
                 right_rotate(t,newnode);
             }
 
         }else{//부모가 조부모의 오른쪽자식이라면
             node_t *y=newnode->parent->parent->left;//삼촌
	     if(y->color==RBTREE_RED){//삼촌컬러가 레드면 case 1
             newnode->parent->color=RBTREE_BLACK;//부모,삼촌의 색과 조부모 색 반전
             y->color=RBTREE_BLACK;
             newnode->parent->parent->color=RBTREE_RED;
             newnode=newnode->parent->parent;
             }else {//삼촌컬러가 블랙일때
                 if (newnode==newnode->parent->left){//꺾여있을때 case3로 만들어줘야겠지
                     newnode=newnode->parent;
                     left_rotate(t,newnode);
                 }
                 newnode->parent->color=RBTREE_BLACK;//여기 이해 아직 잘 안됨 회전시키면?
                 newnode->parent->parent->color=RBTREE_RED;
                 left_rotate(t,newnode->parent->parent);//왼쪽으로 회전
             }
         }
     }
     t->root->color=RBTREE_BLACK;//마지막에 헤드 즉 루트노드의 색은 블랙으로 고정
 }

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
    node_t *newnode=(node_t*)calloc(1,sizeof(node_t));
   // if (!newnode) return NULL;
    newnode->key=key;
    newnode->color=RBTREE_RED;//모든 노드는 레드로 삽입된다.
    newnode->left=newnode->right=newnode->parent=t->nil;//뉴노드의 왼쪽,오른쪽자식 부모는 다 nil 이다
    node_t *y=t->nil;//새로운 y포인터변수 생성 그리고 트리의 끝값 가르킴
    node_t *x=t->root;//새로운 x포인터 변수 생성 그리고 트리의 루트 값 가르킴
    while(x!=t->nil){
	y=x;
	if (key<x->key)
	    x=x->left;//뉴노드의 키값이 루트의 키값보다 작으니 왼쪽 자식으로 내려간다
	else
	    x=x->right;
    }
    newnode->parent=y; //뉴노드의의 부모 그러니까 삽입될 노드의 부모는 루트값이다
    if(y==t->nil)//만약 부모가 끝값이라면?
	t->root=newnode;//이건뭔데 뉴노드의 위치는 구해줬잖아 위에서 그러면 여긴 뭘 구하는거야? 뉴노드의 부모?
    else if (key<y->key)
	y->left=newnode;//부모가 정해졌으니 if문에 따라 그 부모의 왼쪽아니면 오른쪽에 newnode붙이기
    else
	y->right=newnode;
     
    insert_fixup(t,newnode);
    
    return newnode;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
    node_t *cur=t->root;//cur 선언
    while(cur!=t->nil){//닐까지 도달할때
	if(key<cur->key)//키값이 cur의 키값보다 작으면
	    cur=cur->left;//cur은 왼쪽으로 내려감
	else if (key>cur->key)//키값이 cur의 키값보다 크면
	    cur=cur->right;//cur은 오른쪽으로 내려감
	else
	    return cur;//같으면 반환
    }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
    node_t *cur=t->root;
    if(cur==t->nil) return NULL;
    while(cur->left!=t->nil)
	cur=cur->left;
    return cur;
}
node_t *subtree_min(const rbtree *t,node_t *x){//이건 루트의 기준이 아니라 삭제되는 노드의 기준으로 min을 구하기 위해 만들어진 함수.
    while(x->left != t->nil){
	x=x->left;
    }
    return x;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
    node_t *cur=t->root;
    if(cur==t->nil) return NULL;
    while(cur->right!=t->nil)
	cur=cur->right;
    return cur;
}
void transplant(rbtree *t,node_t* u,node_t* v){
    if(u->parent==t->nil)
	t->root=v;
    else if(u->parent->left==u)
	u->parent->left=v;
    else
	u->parent->right=v;
    v->parent=u->parent;

}
void delete_fixup(rbtree *t,node_t *z){
    while (z!=t->root && z->color==RBTREE_BLACK){
	if(z==z->parent->left){//z가 왼쪽자식일때 그리고 변수를 선언할땐 무조껀 중괄호를 붙여라
	    node_t *w=z->parent->right;//형제노드 변수생성
	
	    if(w->color==RBTREE_RED){//case 1
		w->color=RBTREE_BLACK;//형제노드의 컬러를 레드에서 블랙으로 변경 why? 부모와 형제의 색을 바꿔야해서
		z->parent->color=RBTREE_RED;//형제가 레드였는데 부모는 레드일 수 없음 무조껀 블랙이라서 레드로 변경
		left_rotate(t,z->parent);
		w=z->parent->right;//이거 완벽히 이해하진 못했음. 그치만 회전하니까 기존 형제가 부모로 올라가고 형제가 재설정이 되야하는 것 같음
	    }
	    if(w->right->color==RBTREE_BLACK && w->left->color==RBTREE_BLACK){//case2
		w->color=RBTREE_RED;
		z=z->parent;//이건 만약 부모가 루트면 더블블랙이여도 그냥 블랙이 되니까 계속 부모한테 더블블랙을 올리는거다 부모가 루트일때까지.
	    }
	    else{
		if(w->right->color==RBTREE_BLACK){//case 3 형제노드가 블랙이면서 지금 기준이 왼쪽이니까 형제노드의 오른쪽자식의 컬러가 블랙일때.case 3은 일직선이 아니라 꺾여있을때.
		    w->left->color=RBTREE_BLACK;//형제노드와 형제노드의 왼쪽자식의 색깔을 바꿔주고 회전시켜야 한다 case4 를 만들기 위해.
		    w->color=RBTREE_RED;
		    right_rotate(t,w);//오른쪽으로 회전 한번더 봐야할듯
		    w=z->parent->right;
		}
		//case 4 w->left->color==RBTREE_BLACK일때 지금 기준이 왼쪽이면  형제노드의 왼쪽이 블랙일때 즉 레드가 형제노드의 오른쪽일때
		w->color=z->parent->color;//부모의 색이 레드인지 블랙인지 몰라서.
		z->parent->color=RBTREE_BLACK;//형제의 색은 블랙이기 때문에 색을 바꾸니까 블랙으로 설정
		w->right->color=RBTREE_BLACK;//더블블랙의 블랙을 가져옴? 좀 부가설명 필요할듯
		left_rotate(t,z->parent);
		z=t->root;
	    }
	}
	else
	{
	    node_t *w=z->parent->left;
	    if(w->color==RBTREE_RED){
		w->color=RBTREE_BLACK;
		z->parent->color=RBTREE_RED;
		right_rotate(t,z->parent);
		w=z->parent->left;//여기도 반대되네 새로운 형제를 선언해준것.
	    }
	    if(w->right->color==RBTREE_BLACK && w->left->color==RBTREE_BLACK){//case 2
		w->color=RBTREE_RED;
		z=z->parent;//부모가 루트가 될때까지 계속 엑스트라블랙 보내기
	    }else{
		if(w->left->color==RBTREE_BLACK){
		    w->color=RBTREE_RED;
		    w->right->color=RBTREE_BLACK;
		    left_rotate(t,w);
		    w=z->parent->left;///////바꿔줌
		}
		w->color=z->parent->color;
		z->parent->color=RBTREE_BLACK;
		w->left->color=RBTREE_BLACK;
		right_rotate(t,z->parent);
		z=t->root;
	    }
	}
    }
    z->color=RBTREE_BLACK;
}
int rbtree_erase(rbtree *t,node_t* z){
    node_t *x;
    node_t *y=z;//y에다가 삭제할 z 노드의 정보 저장.
    color_t y_original_color=y->color;//자녀가 1개이하일때는 삭제하면 그 삭제하는 아이의 색이 삭제되기때문에.
    if(z->left==t->nil){//만약 삭제할 노드의 왼쪽자식이 닐이라면 오른쪽 자식과 부모를 transplant한다.
	x=z->right;//왜?
	transplant(t,z,z->right);	
    }else if(z->right==t->nil){
	x=z->left;
	transplant(t,z,z->left);
    }
    else{//삭제할 노드의 자식이 둘일때
	y=subtree_min(t,z->right);
	y_original_color = y->color;//자식노드가 두개이상일때는 삭제되는 컬러가 후임자의 색갈이기 때문에 여기서 original_color를 바꿔준다
	x=y->right;//만약 후임자에 후위노드가 있을경우 저장해야하니까 nil도 저장은 일단 함
	if(y!=z->right){
	    transplant(t,y,y->right);
	    y->right=z->right;//이제 노드z를 y로 바꾼거니까 기존 z라이트를 y라이트로 위치수정해준다. 여기서 z가 y로 바뀜
	    y->right->parent=y;//부모도 재조정해준다.
	}else{
	    x->parent=y;
	}
	transplant(t,z,y);
	y->left=z->left;
	y->left->parent=y;//d위의 아까 z->left는 아직 부모가 z임 y로 확실하게 지정해줘야함
	
	y->color=z->color;
	if(x!=t->nil)
	    x->parent=y;
    }
    if (y_original_color==RBTREE_BLACK)
	delete_fixup(t,x);//x가 삭제후에 위치가 깨진곳이야 위의 194번줄이나 185,188번을 보면 알 수 있어.

    return 0;
}


void inorder(node_t *node, const rbtree *t,key_t *arr,size_t n,size_t *idx){
    if(node == t->nil || *idx>=n) return;//노드가 끝이거나
    inorder(node->left,t,arr,n,idx);
    arr[*idx]=node->key;
    (*idx)++;
    inorder(node->right,t,arr,n,idx);
    
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
    size_t idx=0;
    inorder(t->root,t,arr,n,&idx);
  return 0;
}
