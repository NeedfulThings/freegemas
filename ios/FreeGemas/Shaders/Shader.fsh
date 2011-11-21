//
//  Shader.fsh
//  FreeGemas
//
//  Created by Julian Raschke on 21.11.11.
//  Copyright (c) 2011 Raschke & Ludwig GbR. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
