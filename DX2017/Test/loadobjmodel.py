import sys
import os
import numpy as np
import argparse


def parseargs():
    argparser = argparse.ArgumentParser()
    argparser.add_argument('-fn', dest="fn", required=True, help="model file name")
    argparser.add_argument('-cn', dest="cn", default=False, action='store_true',
                           help="if or not to compute normal vector(default=False)")
    argparser.add_argument('-m3d', dest="m3d", default=False, action='store_true',
                           help="if or not to output m3d header(default=False)")
    argparser.add_argument('-lh', dest="lh", default=False, action='store_true',
                           help="if or not left hand system(default=False)")
    argparser.add_argument('-ct', dest="ctan", default=False, action='store_true',
                           help="if or not to compute tangent vector(default=False)")

    return argparser.parse_args()


args = parseargs()
filename = args.fn
path = os.path.dirname(os.path.abspath(filename))
isRHSys = not args.lh
computeNrm = args.cn
computeTan = args.ctan
m3dhead = args.m3d
hasTex = False
hasNrm = False
mtllibname = ""

vertPos = []
vertTex = []
vertNrm = []
vertPosInd = []
vertTexInd = []
vertNrmInd = []
subsetIndStart = []
cSubsetFace = []
meshMaterials = []
material = []
materialmap = {}
dictabc = {}

# final
indices = []
vertices = []
# normarls=[]

iVertInd = 0
cTriangle = 0
cTotalVert = 0
cSubset = 0
cMeshTriangle = 0


def parsemtl():
    if len(mtllibname) == 0:
        print("No material")
        # default material
        matr = {'matName': "default", 'transparent': False,
                'ambColor': [0.2, 0.2, 0.2, 1.0],
                'difColor': [0.8, 0.8, 0.8, 1.0],
                'spcColor': [1.0, 1.0, 1.0, 1.0],
                'emtColor': [0.9, 0.9, 0.9, 1.0],
                'difTexName': "", 'ambTexName': "", 'nrmTexName': ""}
        material.append(matr)
        materialmap[matr['default']] = 0
        return
    # print(mtllibname)
    with open(os.path.join(path, mtllibname)) as f:
        matr = {}
        matrInd = 0
        for line in f:
            line = line.strip()
            if len(line) == 0 or line[0] == '#':
                continue
            if line[0] == 'n':
                if len(line) > 6 and line[:6] == "newmtl":
                    if len(matr) > 0:
                        material.append(matr)
                        materialmap[matr['matName']] = matrInd
                        matrInd += 1
                    matr = {'matName': line[7:], 'transparent': False,
                            'ambColor': [0.2, 0.2, 0.2, 1.0],
                            'difColor': [0.8, 0.8, 0.8, 1.0],
                            'spcColor': [1.0, 1.0, 1.0, 1.0],
                            'emtColor': [0.9, 0.9, 0.9, 1.0],
                            'difTexName': "", 'ambTexName': "", 'nrmTexName': ""}
            elif line[0] == 'K':
                if line[1] == 'a':
                    x, y, z = [float(i) for i in line[3:].split()]
                    matr['ambColor'][0] = x
                    matr['ambColor'][1] = y
                    matr['ambColor'][2] = z
                elif line[1] == 'd':
                    x, y, z = [float(i) for i in line[3:].split()]
                    matr['difColor'][0] = x
                    matr['difColor'][1] = y
                    matr['difColor'][2] = z
                    # kdset=True
                elif line[1] == 's':
                    x, y, z = [float(i) for i in line[3:].split()]
                    matr['spcColor'][0] = x
                    matr['spcColor'][1] = y
                    matr['spcColor'][2] = z
                elif line[1] == 'e':
                    x, y, z = [float(i) for i in line[3:].split()]
                    matr['emtColor'][0] = x
                    matr['emtColor'][1] = y
                    matr['emtColor'][2] = z
            elif line[0] == 'T':
                if line[1] == 'r':
                    tr = float(line[3:])
                    matr['difColor'][3] = 1 - tr
                    matr['ambColor'][3] = 1 - tr
                    if tr > 0:
                        matr['transparent'] = True
            elif line[0] == 'd':
                if line[1] == ' ':
                    opq = float(line[2:])
                    matr['difColor'][3] = opq
                    matr['ambColor'][3] = opq
                    if opq < 1:
                        matr['transparent'] = True
            elif line[0] == 'm':
                if len(line) > 4 and line[:4] == "map_":
                    if line[4:6] == "Kd":
                        matr['difTexName'] = os.path.join(path, line[7:])
                    elif line[4:6] == "Ka":
                        matr['ambTexName'] = os.path.join(path, line[7:])
                    elif line[4:8] == "bump":
                        matr['nrmTexName'] = os.path.join(path, line[9:])
                    elif line[4] == 'd':
                        matr['transparent'] = True
            elif line[0] == 'N':
                if line[1] == 's':
                    matr['spcColor'][3] = float(line[3:])  # specular exponent
            elif line[1] == 'i':
                matr['refrInd'] = float(line[3:])  # refraction index
        material.append(matr)


def splitpnt(abc):
    if ffmt == 1:
        a, b, c = int(abc[0]), 0, 0
    elif ffmt == 2:
        a, b, c = int(abc[0]), int(abc[1]), 0
    elif ffmt == 3:
        a, b, c = int(abc[0]), int(abc[1]), int(abc[2])
    elif ffmt == 4:
        a, b, c = int(abc[0]), 0, int(abc[2])
    vertPosInd.append(a - 1)
    vertTexInd.append(b - 1)
    vertNrmInd.append(c - 1)


with open(filename) as f:
    ffmt = 0
    for line in f:
        line = line.strip()
        if len(line) == 0 or line[0] == '#':
            continue
        if line[0] == 'v':
            if line[1] == ' ':
                x, y, z = [float(i) for i in line[2:].split()]
                if isRHSys:
                    z = -z
                vertPos.append([x, y, z])
            elif line[1] == 't':
                uv_ = [float(i) for i in line[3:].split()]
                if isRHSys:
                    uv_[1] = 1.0 - uv_[1]
                vertTex.append([uv_[0], uv_[1]])
                hasTex = True
            elif line[1] == 'n':
                x, y, z = [float(i) for i in line[3:].split()]
                if isRHSys:
                    z = -z
                vertNrm.append([x, y, z])
                hasNrm = True
        elif line[0] == 'g':
            if line[1] == ' ':
                if cTriangle > 0:
                    cSubsetFace.append(cTriangle)
                    cMeshTriangle += cTriangle
                    cTriangle = 0
                subsetIndStart.append(iVertInd)
                cSubset += 1
                dictabc.clear()
        elif line[0] == 'f':
            if line[1] == ' ':
                face = line[2:].split()
                if ffmt == 0:  # 1:a 2:a/b 3:a/b/c 4:a//c
                    abc = face[0].split('/')
                    ffmt = len(abc)
                    if ffmt == 3 and len(abc[1]) == 0:
                        ffmt = 4
                face1 = face[:3]
                if isRHSys:
                    face1.reverse()
                for f in face1:
                    ind = dictabc.get(f)
                    if ind is None:  # avoid duplicate vertices
                        dictabc[f] = cTotalVert
                        indices.append(cTotalVert)
                        cTotalVert += 1

                        abc = f.split('/')
                        splitpnt(abc)
                    else:
                        indices.append(ind)
                firstInd = indices[iVertInd]
                lastInd = indices[iVertInd + 2]
                if cSubset == 0:
                    subsetIndStart.append(iVertInd)
                    cSubset += 1
                iVertInd += 3
                # cMeshTriangle+=1
                cTriangle += 1

                if len(face) > 3:
                    for f in face[3:]:
                        indices.append(firstInd)
                        indices.append(lastInd)
                        iVertInd += 2
                        ind = dictabc.get(f)
                        if ind is None:  # avoid duplicate vertices
                            dictabc[f] = cTotalVert
                            indices.append(cTotalVert)
                            cTotalVert += 1

                            abc = f.split('/')
                            splitpnt(abc)
                        else:
                            indices.append(ind)
                        lastInd = indices[iVertInd]
                        # cMeshTriangle+=1
                        cTriangle += 1
                        iVertInd += 1
        elif line[0] == 'm':
            if len(line) > 6 and line[:6] == "mtllib":
                mtllibname = line[7:]
        elif line[0] == 'u':
            if len(line) > 6 and line[:6] == "usemtl":
                meshMaterials.append([line[7:], -1])

if cTriangle > 0:
    cSubsetFace.append(cTriangle)
    cMeshTriangle += cTriangle
    cTriangle = 0
subsetIndStart.append(iVertInd)

if subsetIndStart[1] == 0:
    if len(subsetIndStart) > 2:
        subsetIndStart = subsetIndStart[0:1] + subsetIndStart[2:]
    else:
        subsetIndStart = subsetIndStart[0:1]
    cSubset -= 1
if not hasTex:
    vertTex.append([0, 0])
if not hasNrm:
    vertNrm.append([0, 0, 0])

parsemtl()

for i in range(cSubset):
    # hasMat = False
    ind = materialmap.get(meshMaterials[i][0])
    if ind is not None:
        meshMaterials[i][1] = ind

for i in range(cTotalVert):
    p = vertPos[vertPosInd[i]]
    t = vertTex[vertTexInd[i]]
    n = vertNrm[vertNrmInd[i]]
    tanu = [0, 0, 0]
    vertices.append([p, t, n, tanu])


def vadd3(a, b):
    a[0] += b[0]
    a[1] += b[1]
    a[2] += b[2]


def vnrm(a):
    mang = np.linalg.norm(a)
    for i in range(len(a)):
        a[i] /= mang


if computeNrm:
    print('compute normals')
    for i in range(cMeshTriangle):
        i0, i1, i2 = indices[i * 3], indices[i * 3 + 1], indices[i * 3 + 2]
        v0 = vertices[i0]
        v1 = vertices[i1]
        v2 = vertices[i2]
        e0 = np.array(v1[0]) - np.array(v0[0])
        e1 = np.array(v2[0]) - np.array(v0[0])
        nrm = np.cross(e0, e1)
        vadd3(v0[2], nrm)
        vadd3(v1[2], nrm)
        vadd3(v2[2], nrm)
    # normalize
    for i in range(cTotalVert):
        vnrm(vertices[i][2])

pinv = False
singular = False
if computeTan and hasTex:
    print("compute tangent U")
    for i in range(cMeshTriangle):
        i0, i1, i2 = indices[i * 3], indices[i * 3 + 1], indices[i * 3 + 2]
        v0 = vertices[i0]
        v1 = vertices[i1]
        v2 = vertices[i2]
        e0 = np.array(v1[0]) - np.array(v0[0])
        e1 = np.array(v2[0]) - np.array(v0[0])
        u0 = np.array(v1[1]) - np.array(v0[1])
        u1 = np.array(v2[1]) - np.array(v0[1])
        em = np.array([e0, e1])
        um = np.array([u0, u1])
        try:
            uminv = np.linalg.inv(um)
        except Exception as e:
            print("warning:", e)
            singular = True
            if not pinv:
                break
            uminv = np.linalg.pinv(um)
        tm = np.dot(uminv, em)
        tanu = tm[0]
        # vnrm(tanu)
        vadd3(v0[3], tanu)
        vadd3(v1[3], tanu)
        vadd3(v2[3], tanu)

    if not singular or pinv:
        for i in range(cTotalVert):
            vnrm(vertices[i][3])

ext = '.txt'
if m3dhead:
    ext = '.m3d'
with open(os.path.join(path, os.path.basename(filename).split('.')[0] + ext), 'w') as f:
    if m3dhead:
        f.write('***************m3d-File-Header***************\n')
        f.write('#Materials %d\n'
                '#Vertices %d\n'
                '#Triangles %d\n'
                '#Bones 0\n'
                '#AnimationClips 0\n'
                % (len(meshMaterials), cTotalVert, cMeshTriangle))
        f.write('\n')
        f.write('***************Materials*********************\n')
        for _, ind in meshMaterials:
            mt = material[ind]
            f.write('Ambient: %.6f %.6f %.6f\n'
                    'Diffuse: %.6f %.6f %.6f\n'
                    'Specular: %.6f %.6f %.6f\n'
                    'SpecPower: %d\n'
                    'Reflectivity: 0 0 0\n'
                    'AlphaClip: 1\n'
                    'Effect: Normal\n'
                    'DiffuseMap: %s\n'
                    'NormalMap: %s\n'
                    % (mt['ambColor'][0], mt['ambColor'][1], mt['ambColor'][2],
                       mt['difColor'][0], mt['difColor'][1], mt['difColor'][2],
                       mt['spcColor'][0], mt['spcColor'][1], mt['spcColor'][2], mt['spcColor'][3],
                       mt['difTexName'], mt['nrmTexName']))
            f.write('\n')

        f.write('***************SubsetTable*******************\n')
        cFace = 0
        for i in range(cSubset):
            # cV=subsetIndStart[i+1]-subsetIndStart[i]
            f.write('SubSetID: %d VertexStart: %d VertexCount: %d FaceStart: %d FaceCount: %d\n'
                    % (i, subsetIndStart[i], subsetIndStart[i + 1] - subsetIndStart[i], cFace, cSubsetFace[i]))
            cFace += cSubsetFace[i]
        f.write('\n')
        f.write('***************Vertices**********************\n')
        for v in vertices:
            p, t, n, tan = v
            f.write('Position: %.6f %.6f %.6f\n'
                    'Tangent: %.6f %.6f %.6f\n'
                    'Normal: %.6f %.6f %.6f 1.0\n'
                    'Tex-Coords: %.6f %.6f\n'
                    % (p[0], p[1], p[2],
                       tan[0], tan[1], tan[2],
                       n[0], n[1], n[2],
                       t[0], t[1]))
            f.write("\n")
        f.write("***************Triangles*********************\n")
        for i in range(cMeshTriangle):
            for j in range(3):
                f.write('%d ' % (indices[i * 3 + j]))
            f.write('\n')
    else:
        f.write('VertexCount: %d\n' % cTotalVert)
        f.write('TriangleCount: %d\n' % cMeshTriangle)
        for i in range(len(material)):
            if len(material[i]["difTexName"]) > 0:
                break
        if i == len(material):
            i = 0
        mt = material[i]
        f.write('Ambient: %.6f %.6f %.6f %.6f\n'
                'Diffuse: %.6f %.6f %.6f %.6f\n'
                'Specular: %.6f %.6f %.6f %.6f\n'
                'DiffuseMap: %s\n'
                % (mt['ambColor'][0], mt['ambColor'][1], mt['ambColor'][2], mt['ambColor'][3],
                   mt['difColor'][0], mt['difColor'][1], mt['difColor'][2], mt['difColor'][3],
                   mt['spcColor'][0], mt['spcColor'][1], mt['spcColor'][2], mt['spcColor'][3],
                   mt['difTexName']))
        f.write('VertexList (pos, normal, texC)\n{\n')
        for v in vertices:
            data = v[0] + v[2] + v[1]
            for x in data:
                f.write(' %.6f' % (x))
            f.write('\n')
        f.write('}\n')
        f.write('TriangleList\n{\n')
        for i in range(cMeshTriangle):
            for j in range(3):
                f.write(' %d' % (indices[i * 3 + j]))
            f.write('\n')
        f.write('}\n')
