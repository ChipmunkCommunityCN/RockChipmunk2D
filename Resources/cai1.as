package
{
	// import quick box2d
	import com.actionsnippet.qbox.*;

    // other stuff
    import flash.utils.Dictionary;
	import flash.utils.getDefinitionByName;


    public class PhysicsData extends Object
	{
		// ptm ratio
        public var ptm_ratio:Number = 2;
		
		// the physcis data 
		var dict:Dictionary;
		
        public function createBody(name:String, sim:QuickBox2D, xPos:Number, yPos:Number):QuickObject
        {
			var fixture:Array = dict[name][0];

           	return sim.addPoly({
                        x:xPos, y:yPos, 
                        density:fixture[0], 
                        friction:fixture[1], 
                        restitution:fixture[2],
                        categoryBits: fixture[3],
                        maskBits:fixture[4],
                        groupIndex: fixture[5],
                        verts:fixture[8],
						skin:getDefinitionByName(name)
                        });							
			}
		
        public function PhysicsData(): void
		{
			dict = new Dictionary();
			

			dict["cai1"] = [

										[
											// density, friction, restitution
                                            2, 0, 0,
                                            // categoryBits, maskBits, groupIndex, isSensor
											1, 65535, 0, false,
											'POLYGON',
											[

                                                [   39/ptm_ratio, 7/ptm_ratio  ,  58/ptm_ratio, 0/ptm_ratio  ,  95/ptm_ratio, 17/ptm_ratio  ,  38/ptm_ratio, 27/ptm_ratio  ,  30/ptm_ratio, 19/ptm_ratio  ] ,
                                                [   97/ptm_ratio, 81/ptm_ratio  ,  113/ptm_ratio, 50/ptm_ratio  ,  121/ptm_ratio, 65/ptm_ratio  ,  120/ptm_ratio, 81/ptm_ratio  ,  110/ptm_ratio, 88/ptm_ratio  ] ,
                                                [   38/ptm_ratio, 27/ptm_ratio  ,  95/ptm_ratio, 17/ptm_ratio  ,  116/ptm_ratio, 34/ptm_ratio  ,  113/ptm_ratio, 50/ptm_ratio  ,  97/ptm_ratio, 81/ptm_ratio  ,  73/ptm_ratio, 83/ptm_ratio  ,  36/ptm_ratio, 57/ptm_ratio  ,  28/ptm_ratio, 44/ptm_ratio  ] ,
                                                [   78/ptm_ratio, 89/ptm_ratio  ,  73/ptm_ratio, 83/ptm_ratio  ,  97/ptm_ratio, 81/ptm_ratio  ,  92/ptm_ratio, 89/ptm_ratio  ] ,
                                                [   27/ptm_ratio, 78/ptm_ratio  ,  44/ptm_ratio, 95/ptm_ratio  ,  20/ptm_ratio, 121/ptm_ratio  ,  8/ptm_ratio, 114/ptm_ratio  ,  1/ptm_ratio, 100/ptm_ratio  ] ,
                                                [   36/ptm_ratio, 57/ptm_ratio  ,  73/ptm_ratio, 83/ptm_ratio  ,  44/ptm_ratio, 95/ptm_ratio  ,  27/ptm_ratio, 78/ptm_ratio  ]
											]
										]

									];

		}
	}
}
