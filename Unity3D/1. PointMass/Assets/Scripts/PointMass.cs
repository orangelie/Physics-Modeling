using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PointMass : MonoBehaviour
{
    public float animateSpeedx = 2.0f;

    public GameObject Camera;        // gameObject of Camera
    private float mouseX = 0.0f;
    private float mouseY = 0.0f;

    private bool forceApplied = false;
    private bool Restart = false;

    // ---------------------------------------------------------------------------------------------------

    public GameObject enemy;        // gameObject of Enemy
    private float r = -1.0f;        // radius of sphere which this object.
    private float e_r = -1.0f;      // radius of sphere which enemy.

    private bool collided = false;
    private float totalTime = 0.0f;

    // ---------------------------------------------------------------------------------------------------

    // ME
    [SerializeField]
    public Vector3 force = Vector3.zero;

    [SerializeField]
    public float mass = 10.0f;
    private Vector3 acceleration = Vector3.zero;
    private Vector3 velocity = Vector3.zero;
    private Vector3 position = Vector3.zero;
    private float elasticity = 0.9f;

    // ENEMY
    [SerializeField]
    public Vector3 e_force = Vector3.zero;

    [SerializeField]
    public float e_mass = 10.0f;
    private Vector3 e_acceleration = Vector3.zero;
    private Vector3 e_velocity = Vector3.zero;
    private Vector3 e_position = Vector3.zero;
    private float e_elasticity = 0.9f;

    // ---------------------------------------------------------------------------------------------------

    void Start()
    {
        Cursor.visible = false;

        position = transform.position;
        e_position = enemy.transform.position;

        r = GetComponent<SphereCollider>().radius *
            Mathf.Max(transform.lossyScale.x, transform.lossyScale.y, transform.lossyScale.z);

        e_r = enemy.GetComponent<SphereCollider>().radius *
            Mathf.Max(enemy.transform.lossyScale.x, enemy.transform.lossyScale.y, enemy.transform.lossyScale.z);
    }

    void Update()
    {
        GetKeyEvent();

        totalTime += Time.deltaTime * animateSpeedx;

        if (!forceApplied)
        {
            forceApplied = true;
        }
        else
        {
            force.Set(0.0f, 0.0f, 0.0f);
            e_force.Set(0.0f, 0.0f, 0.0f);
        }

        Vector3 dist = CollisionDetection();
        if(collided)
        {
            CollisionResponse(dist, totalTime);
            collided = false;
        }
        ApplyForce(totalTime);

        transform.position = position;
        enemy.transform.position = e_position;
    }

    void GetKeyEvent()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
            Application.Quit();

        if (Input.GetKeyDown(KeyCode.B) && !Restart)
        {
            force.Set(5.0f, 0.0f, 0.0f);
            e_force.Set(0.0f, -5.0f, 0.0f);
            totalTime = 0.0f;
            forceApplied = false;
            Restart = true;
        }

        mouseX += Input.GetAxisRaw("Mouse X") * 3.0f;
        mouseY += Input.GetAxisRaw("Mouse Y") * 3.0f;

        mouseY = Mathf.Clamp(mouseY, -60.0f, 60.0f);

        Camera.transform.localEulerAngles = new Vector3(-mouseY, mouseX, 0.0f);

        Camera.transform.rotation = Quaternion.Slerp(Camera.transform.rotation,
            Quaternion.Euler(Camera.transform.localEulerAngles), Time.deltaTime);
    }

    private void CollisionResponse(Vector3 dist, float timeInterval)
    {
        Vector3 unitNormal = Vector3.Normalize(dist);

        float vel1 = Vector3.Dot(velocity, unitNormal);
        float vel2 = Vector3.Dot(e_velocity, unitNormal);

        float averageE = (elasticity * e_elasticity) / 2;

        float magvel1 = 
            (((mass - 
            (averageE * e_mass)) * vel1) +
            ((1 + averageE) * e_mass * vel2)) /
            (mass + e_mass);

        float magvel2 =
            (((e_mass -
            (averageE * mass)) * vel2) +
            ((1 + averageE) * mass * vel1)) /
            (mass + e_mass);

        velocity = (magvel1 - vel1) * unitNormal + velocity;
        e_velocity = (magvel2 - vel2) * unitNormal + e_velocity;

        Vector3 acceleration1 = velocity / timeInterval;
        Vector3 acceleration2 = e_velocity / timeInterval;

        force = (acceleration1 * mass);
        e_force = (acceleration2 * e_mass);
    }

    private void ApplyForce(float timeInterval)
    {
        // F = ma -> a = F/m
        acceleration = force / mass;

        // a = dv/dt -> (integral calc) -> v = at
        velocity += acceleration * timeInterval;

        // v = dx/dt -> (integral calc) -> x = vt
        position += velocity * timeInterval;



        // F = ma -> a = F/m
        e_acceleration = e_force / e_mass;

        // a = dv/dt -> (integral calc) -> v = at
        e_velocity += e_acceleration * timeInterval;

        // v = dx/dt -> (integral calc) -> x = vt
        e_position += e_velocity * timeInterval;
    }

    private Vector3 CollisionDetection()
    {
        Vector3 distance = (transform.position - enemy.transform.position);

        float magnitude = NormSquared(distance);

        float radiusSumm = e_r + r;
        radiusSumm *= radiusSumm;

        // collision triggered !!
        if (magnitude <= radiusSumm)
        {
            collided = true;
            Debug.Log("It collided !");
        }

        return distance;
    }

    private float Norm(Vector3 vec)
    {
        return Mathf.Sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }

    private float NormSquared(Vector3 vec)
    {
        return (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }
}